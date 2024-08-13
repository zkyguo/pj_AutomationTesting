#include "Automation/SimpleProjectAutomation.h"

#include "AutomationTesting.h"

namespace ProjectAutomationTest
{
	void SetMouse(const FVector2D& InPos)
	{
		if(IsInGameThread())
		{
			FSlateApplication::Get().SetCursorPos(InPos);
		}
		else
		{
			//if we are in other thread, let the main tread finish before start 
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
