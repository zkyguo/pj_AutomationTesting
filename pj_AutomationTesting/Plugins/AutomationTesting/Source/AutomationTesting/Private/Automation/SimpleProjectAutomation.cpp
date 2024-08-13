#include "Automation/SimpleProjectAutomation.h"
#include "AutomationDriverTypeDefs.h"
#include "AutomationTesting.h"
#include "IAutomationDriver.h"
#include "IAutomationDriverModule.h"
#include "IDriverElement.h"
#include "IDriverSequence.h"
#include "LocateBy.h"

namespace ProjectAutomationTest
{
	void StartAutomationTest(const TArray<TSharedPtr<FProjectTestBaseData>>& InputDatas,
		const TArray<TSharedPtr<FProjectTestBaseData>>& AxialInputData)
	{
		if (IsInGameThread())
		{
			//Stop all the work of IAutomationDriverModule 
			if (IAutomationDriverModule::Get().IsEnabled())
			{
				IAutomationDriverModule::Get().Disable();
			}
			IAutomationDriverModule::Get().Enable();

			//Start play Input recording
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
				[InputDatas]()
				{
					FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
					FAutomationDriverPtr Driver = IAutomationDriverModule::Get().CreateDriver();

					for (auto& Tmp : InputDatas)
					{
						float WaitTime = Tmp->DurationTime - InModule.GetCurrentTime();

						//if it still recording, wait until it finishes
						if(WaitTime > 0.1f)
						{
							Driver->Wait(FTimespan::FromSeconds(WaitTime));
						}

						if(TSharedPtr<FProjectTestKey> TestKey = StaticCastSharedPtr<FProjectTestKey>(Tmp))
						{
							ShowMouseCursor(TestKey->bShowMouseCursor);
							SetMouse(TestKey->CursorPos);

							FDriverElementRef ElementUnderCursor = Driver->FindElement(By::Cursor());

							if (TestKey->Key == EKeys::RightMouseButton || TestKey->Key == EKeys::LeftMouseButton ||
								TestKey->Key == EKeys::MiddleMouseButton || TestKey->Key == EKeys::ThumbMouseButton ||
								TestKey->Key == EKeys::ThumbMouseButton2 || TestKey->Key == EKeys::MouseWheelAxis)
							{
								if (TestKey->Key == EKeys::MouseWheelAxis)
								{
									ElementUnderCursor->ScrollBy(TestKey->Delta);
								}
								else
								{
									if (TestKey->InputEvent == EInputEvent::IE_DoubleClick)
									{
										ElementUnderCursor->DoubleClick(TestKey->MouseButtons);
									}
									else if (TestKey->InputEvent == EInputEvent::IE_Pressed)
									{
										TSharedRef<IDriverSequence, ESPMode::ThreadSafe> Sequence = Driver->CreateSequence();

										Sequence->Actions().Focus(ElementUnderCursor);
										Sequence->Actions().Click(TestKey->MouseButtons);
										Sequence->Actions().Press(TestKey->MouseButtons);

										Sequence->Perform();
									}
									else if (TestKey->InputEvent == EInputEvent::IE_Released)
									{
										ElementUnderCursor->Release(TestKey->MouseButtons);
									}
								}
							}
							else
							{
								//Check if its about Ui simulation
								if (ElementUnderCursor->IsVisible())
								{
									if (TestKey->Key.IsValid())
									{
										if (TestKey->InputEvent == EInputEvent::IE_Pressed)
										{
											ElementUnderCursor->Press(TestKey->Key);
										}
										else if (TestKey->InputEvent == EInputEvent::IE_Released)
										{
											ElementUnderCursor->Release(TestKey->Key);
										}
									}
								}
								else
								{
									SimulateKeyPress(TestKey->Key, TestKey->InputEvent);
								}
							}
						}
					}

					//If all record input ends, stop play input 
					FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
						{

						}, TStatId(), NULL, ENamedThreads::GameThread);

					FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
				});

			//Start play axial input recording
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
				[AxialInputData]()
				{
					FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");

					for (auto& Tmp : AxialInputData)
					{
						float WaitTime = Tmp->DurationTime - InModule.GetCurrentTime();

						if (WaitTime > 0.f)
						{
							FPlatformProcess::Sleep(WaitTime);
						}

						if (TSharedPtr<FProjectTestInputAxial> TestKey = StaticCastSharedPtr<FProjectTestInputAxial>(Tmp))
						{
							SimulateKeyPress(TestKey->Key, TestKey->Value, TestKey->DeltaTime);
						}
					}

					FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
						{
							
						}, TStatId(), NULL, ENamedThreads::GameThread);

					FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

				});
		}
	}

	void SimulateKeyPress(FKey key, EInputEvent InputEvent)
	{
	}

	void SimulateKeyPress(FKey key, const FVector2D& InValue, float DeltaTime)
	{
	}

	void SetMouse(const FVector2D& InPos)
	{
		if(IsInGameThread())
		{
			FSlateApplication::Get().SetCursorPos(InPos);
		}
		else
		{
			//if we are in other thread, dont start it and wait for main thread finish
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
				{
					FSlateApplication::Get().SetCursorPos(InPos);
				}, TStatId(), NULL, ENamedThreads::GameThread);

			FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
			
		}
	}

	void ShowMouseCursor(bool bIsShow)
	{
		auto HandleMouseCursor = [&]()
			{
				FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");

				if(UWorld* InWorld = InModule.GetWorld())
				{
					InWorld->GetFirstPlayerController()->SetShowMouseCursor(bIsShow);

					FInputModeGameAndUI InputMode;

					if(bIsShow)
					{
						InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
						InputMode.SetHideCursorDuringCapture(false);
					}
					else
					{
						InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
						InputMode.SetHideCursorDuringCapture(true);
					}

					InWorld->GetFirstPlayerController()->SetInputMode(InputMode);
				}
			};

		if (IsInGameThread())
		{
			HandleMouseCursor();
		}
		else
		{
			//if we are in other thread, let the main tread finish before start 
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
				{
					HandleMouseCursor();
				}, TStatId(), NULL, ENamedThreads::GameThread);

			FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		}
	}
}

	
