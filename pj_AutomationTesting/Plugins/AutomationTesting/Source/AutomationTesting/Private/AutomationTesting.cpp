// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTesting.h"
#include "AutomationTestType.h"
#include "Windows/WindowsWindow.h"
#include "InputCoreTypes.h"
#include "Stream/SimpleProjectStream.h"
#include "Misc/ICompressionFormat.h"
#include "Automation/SimpleProjectAutomation.h"
#include "Settings/SimpleProjectTestSettings.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "AutomationTestLog.h"
#include "IAutomationDriver.h"
#include "IAutomationDriverModule.h"
#include "zlib.h"
#include "Core/AutomationProjectTestMethod.h"


THIRD_PARTY_INCLUDES_START
#include <Windows.h>
#include <windowsx.h>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "FAutomationTestingModule"

FAutomationTestingModule::FAutomationTestingModule()
{
	MixCount = 0;
	TestState = EProjectTestState::Free;
	FMemory::Memset(ModifierKeyState, 0, (int32)EProjectTestModifierKey::Count + 1);
}

void FAutomationTestingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FAutomationTestingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

void FAutomationTestingModule::StartRecording()
{
	FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
	check(WindowsApplication);
	WindowsApplication->AddMessageHandler(*this);

	TestState = EProjectTestState::Recording;

	CurrentTime = 0.f;
	InputData.Empty();
	AxialInputData.Empty();
}

void FAutomationTestingModule::StopRecording()
{
	if (FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get())
	{
		WindowsApplication->RemoveMessageHandler(*this);
	}
	TestState = EProjectTestState::Free;
	CurrentTime = 0.f;
}

void FAutomationTestingModule::StartPlay()
{
	if(InputData.Num() > 0 || AxialInputData.Num() > 0)
	{
		CurrentTime = 0.f;
		TestState = EProjectTestState::Playing;
		MixCount = 0;

		ProjectAutomationTest::StartAutomationTest(InputData, AxialInputData);
	}
}

void FAutomationTestingModule::StopPlay()
{
	IAutomationDriverModule::Get().Disable();
	MixCount = 0;
	TestState = EProjectTestState::Free;
}

TStatId FAutomationTestingModule::GetStatId() const
{
	return TStatId();
}

void FAutomationTestingModule::Tick(float DeltaTime)
{
	if(TestState != EProjectTestState::Free)
	{
		CurrentTime += DeltaTime;
	}
}

bool FAutomationTestingModule::ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam,
	int32& OutResult)
{
	OutResult = 1;

	switch (msg)
	{
		case WM_CHAR:
		{
			if(const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if(!InSettings->bCustomInput)
				{
					const TCHAR Character = IntCastChecked<TCHAR>(wParam);
					const bool bIsRepeat = (lParam & 0x40000000) != 0;
					OnKeyChar(Character, bIsRepeat);
				}
				
			}
			return true;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
					const int32 Win32Key = IntCastChecked<int32>(wParam);
					int32 ActualKey = Win32Key;

					bool bIsRepeat = (lParam & 0x40000000) != 0;

					switch (Win32Key)
					{
					case VK_MENU:
					{
						if ((lParam & 0x1000000) == 0)
						{
							ActualKey = VK_LMENU;
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::LeftAlt];
							ModifierKeyState[EProjectTestModifierKey::LeftAlt] = true;
						}
						else
						{
							ActualKey = VK_RMENU;
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::RightAlt];
							ModifierKeyState[EProjectTestModifierKey::RightAlt] = true;
						}

						break;
					}
					case VK_CONTROL:
					{
						if ((lParam & 0x1000000) == 0)
						{
							ActualKey = VK_LCONTROL;
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::LeftControl];
							ModifierKeyState[EProjectTestModifierKey::LeftControl] = true;
						}
						else
						{
							ActualKey = VK_RCONTROL;
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::RightControl];
							ModifierKeyState[EProjectTestModifierKey::RightControl] = true;
						}
						break;
					}
					case VK_SHIFT:
					{
						ActualKey = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
						if (ActualKey == VK_LSHIFT)
						{
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::LeftShift];
							ModifierKeyState[EProjectTestModifierKey::LeftShift] = true;
						}
						else
						{
							bIsRepeat = ModifierKeyState[EProjectTestModifierKey::RightShift];
							ModifierKeyState[EProjectTestModifierKey::RightShift] = true;
						}
						break;
					}
					case VK_CAPITAL:
						ModifierKeyState[EProjectTestModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
						break;
					default:
						// No translation needed
						break;
					}
					uint32 CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);
					const bool Result = OnKeyDown(ActualKey, CharCode, bIsRepeat);

					if (Result || wParam != WM_SYSKEYDOWN)
					{
						// Handled
						return true;
					}
				}
			}
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
			int32 Win32Key = IntCastChecked<int32>(wParam);

			int32 ActualKey = Win32Key;

			bool bModifierKeyReleased = false;

			switch (Win32Key)
			{
				
			case VK_MENU:
				// Differentiate between left and right alt
				if ((lParam & 0x1000000) == 0)
				{
					ActualKey = VK_LMENU;
					ModifierKeyState[EProjectTestModifierKey::LeftAlt] = false;
				}
				else
				{
					ActualKey = VK_RMENU;
					ModifierKeyState[EProjectTestModifierKey::RightAlt] = false;
				}
				break;
			case VK_CONTROL:
				// Differentiate between left and right control
				if ((lParam & 0x1000000) == 0)
				{
					ActualKey = VK_LCONTROL;
					ModifierKeyState[EProjectTestModifierKey::LeftControl] = false;
				}
				else
				{
					ActualKey = VK_RCONTROL;
					ModifierKeyState[EProjectTestModifierKey::RightControl] = false;
				}
				break;
			case VK_SHIFT:
				// Differentiate between left and right shift
				ActualKey = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
				if (ActualKey == VK_LSHIFT)
				{
					ModifierKeyState[EProjectTestModifierKey::LeftShift] = false;
				}
				else
				{
					ModifierKeyState[EProjectTestModifierKey::RightShift] = false;
				}
				break;
			case VK_CAPITAL:
				ModifierKeyState[EProjectTestModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
				break;
			default:
				// No translation needed
				break;

					}

					uint32 CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);

					const bool bIsRepeat = false;

					const bool Result = OnKeyUp(ActualKey, CharCode, bIsRepeat);

					if (Result || wParam != WM_SYSKEYUP)
					{
						// Handled
						return true;
					}
				}
			}
			break;
		}
		// Mouse Button Down
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
					POINT CursorPoint;
					CursorPoint.x = GET_X_LPARAM(lParam);
					CursorPoint.y = GET_Y_LPARAM(lParam);

					EMouseButtons::Type MouseButton = EMouseButtons::Invalid;

					bool bDoubleClick = false;
					bool bMouseUp = false;

					switch (msg)
					{
					case WM_LBUTTONDBLCLK:
						bDoubleClick = true;
						MouseButton = EMouseButtons::Left;
						break;
					case WM_LBUTTONUP:
						bMouseUp = true;
						MouseButton = EMouseButtons::Left;
						break;
					case WM_LBUTTONDOWN:
						MouseButton = EMouseButtons::Left;
						break;
					case WM_MBUTTONDBLCLK:
						bDoubleClick = true;
						MouseButton = EMouseButtons::Middle;
						break;
					case WM_MBUTTONUP:
						bMouseUp = true;
						MouseButton = EMouseButtons::Middle;
						break;
					case WM_MBUTTONDOWN:
						MouseButton = EMouseButtons::Middle;
						break;
					case WM_RBUTTONDBLCLK:
						bDoubleClick = true;
						MouseButton = EMouseButtons::Right;
						break;
					case WM_RBUTTONUP:
						bMouseUp = true;
						MouseButton = EMouseButtons::Right;
						break;
					case WM_RBUTTONDOWN:
						MouseButton = EMouseButtons::Right;
						break;
					case WM_XBUTTONDBLCLK:
						bDoubleClick = true;
						MouseButton = (HIWORD(wParam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
						break;
					case WM_XBUTTONUP:
						bMouseUp = true;
						MouseButton = (HIWORD(wParam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
						break;
					case WM_XBUTTONDOWN:
						MouseButton = (HIWORD(wParam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
						break;
					default:
						check(0);
					}

					const FVector2D APPCursorPoint = FSlateApplication::Get().GetCursorPos();

					if (bMouseUp)
					{
						return OnMouseUp(MouseButton, APPCursorPoint) ? 0 : 1;
					}
					else if (bDoubleClick)
					{
						OnMouseDoubleClick(MouseButton, APPCursorPoint);
					}
					else
					{
						OnMouseDown(MouseButton, APPCursorPoint);
					}

					
				}
			}
			return true;
		}
		case WM_INPUT:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
					OnMouseMove();
				}
			}
			break;
		}
		case WM_NCMOUSEMOVE:
		case WM_MOUSEMOVE:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
					OnMouseMove();
				}
			}
			break;
		}
		case WM_TOUCH:
		{
			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (const USimpleProjectTestSettings* InSettings = GetDefault<USimpleProjectTestSettings>())
			{
				if (!InSettings->bCustomInput)
				{
					const float SpinFactor = 1 / 120.0f;
					const SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

					POINT CursorPoint;
					CursorPoint.x = GET_X_LPARAM(lParam);
					CursorPoint.y = GET_Y_LPARAM(lParam);

					const FVector2D CursorPos(CursorPoint.x, CursorPoint.y);

					return OnMouseWheel(static_cast<float>(WheelDelta) * SpinFactor, FSlateApplication::Get().GetCursorPos());
				}
			}
			return false;
		}
		case WM_MOUSEACTIVATE:
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_SETTINGCHANGE:
		case WM_CLOSE:
		{
			break;
		}
		default:
			break;

	}

	return false;
}

bool FAutomationTestingModule::OnMouseUp(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos)
{

	BuildInput(
		AutomationProjectTestMethod::GetMouseKey(MouseButton),
		EInputEvent::IE_Released,
		InCursorPos,
		false,
		0.f,
		MouseButton);


	UE_LOG(LogTemp, Display, TEXT("OnMouseUP : Cursor = %s"), *InCursorPos.ToString());
	return false;
}

bool FAutomationTestingModule::OnMouseDoubleClick(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos)
{
	BuildInput(
		AutomationProjectTestMethod::GetMouseKey(MouseButton),
		EInputEvent::IE_DoubleClick,
		InCursorPos,
		false,
		0.f,
		MouseButton);

	UE_LOG(LogTemp, Display, TEXT("OnMouseDoubleClick : Cursor = %s"), *InCursorPos.ToString());
	return false;
}

bool FAutomationTestingModule::OnMouseDown(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos)
{
	UE_LOG(LogTemp, Display, TEXT("OnMouseDown : Cursor = %s"), *InCursorPos.ToString());
	return false;
}

bool FAutomationTestingModule::OnMouseWheel(const float Delta, const FVector2D& InCursorPos)
{
	BuildInput(
		EKeys::MouseWheelAxis,
		EInputEvent::IE_Axis,
		InCursorPos,
		false,
		Delta,
		EMouseButtons::Middle);

	UE_LOG(LogTemp, Display, TEXT("OnMouseWheel : Delta = %f"), Delta);
	return false;
}

bool FAutomationTestingModule::OnMouseMove()
{
	if (World.IsValid() && World->GetFirstPlayerController())
	{
		FVector2D Delta;
		World->GetFirstPlayerController()->GetInputMouseDelta(Delta.X, Delta.Y);

		if (Delta.Size() > 0)
		{
			BuildInputAxial(EKeys::Mouse2D, Delta);

			UE_LOG(LogTemp, Display, TEXT("OnMouseMove DeltaX=%f,DeltaY=%f"), Delta.X, Delta.Y);
		}
	}

	return false;
}

bool FAutomationTestingModule::OnKeyUp(const int32 KeyCode, const uint32 CharacterCode, bool bIsRepeat)
{

	BuildInput(
		AutomationProjectTestMethod::KeyCodeToFKey(KeyCode),
		EInputEvent::IE_Released,
		FSlateApplication::Get().GetCursorPos(),
		bIsRepeat);

	UE_LOG(LogTemp, Display, TEXT("OnKeyUp : Key = %i, isRepeat = %i"), KeyCode, bIsRepeat);
	return false;
}

bool FAutomationTestingModule::OnKeyDown(const int32 KeyCode, const uint32 CharacterCode, bool bIsRepeat)
{

	BuildInput(
		AutomationProjectTestMethod::KeyCodeToFKey(KeyCode),
		EInputEvent::IE_Pressed,
		FSlateApplication::Get().GetCursorPos(),
		bIsRepeat);

	UE_LOG(LogTemp, Display, TEXT("OnKeyDown : Key = %i, isRepeat = %i"), KeyCode, bIsRepeat);
	return false;
}

bool FAutomationTestingModule::OnKeyChar(const TCHAR Character, const bool IsRepeat)
{

	FKey K = AutomationProjectTestMethod::GetFKeyFromCharCode(Character);

	UE_LOG(LogTemp, Display, TEXT("IsRepeat=%i,KeyChar %s"), IsRepeat, *K.ToString());

	return false;
}

void FAutomationTestingModule::BuildInputAxial(const FKey& InKey, const FVector2D& Value)
{
	if (World.IsValid())
	{
		if (IsRecording())
		{
			TSharedPtr<FProjectTestInputAxial> ProjectTestInputAxial = MakeShareable(new FProjectTestInputAxial());
			ProjectTestInputAxial->InputType = EProjectTestInputType::InputAxial;
			ProjectTestInputAxial->Key = InKey;
			ProjectTestInputAxial->DurationTime = CurrentTime;
			ProjectTestInputAxial->DeltaTime = World->GetDeltaSeconds();
			ProjectTestInputAxial->Value = Value;

			AxialInputData.Add(ProjectTestInputAxial);
		}
	}
}

void FAutomationTestingModule::BuildInput(const FKey& InKey, EInputEvent InputEvent, const FVector2D& InCursorPos,
	bool bIsRepeat, float InDelta, EMouseButtons::Type MouseButtons)
{
	if (World.IsValid())
	{
		if (IsRecording())
		{
			TSharedPtr<FProjectTestKey> ProjectTestKey = MakeShareable(new FProjectTestKey());
			ProjectTestKey->InputType = EProjectTestInputType::Input;
			ProjectTestKey->Key = InKey;
			ProjectTestKey->InputEvent = bIsRepeat ? EInputEvent::IE_Repeat : InputEvent;
			ProjectTestKey->bIsRepeat = bIsRepeat;
			ProjectTestKey->DurationTime = CurrentTime;
			ProjectTestKey->CursorPos = InCursorPos;
			ProjectTestKey->Delta = InDelta;
			ProjectTestKey->MouseButtons = MouseButtons;

			FMemory::Memcpy(ProjectTestKey->ModifierKeyState, ModifierKeyState, (int32)EProjectTestModifierKey::Count + 1);

			if(World->GetFirstPlayerController())
			{
				ProjectTestKey->bShowMouseCursor = World->GetFirstPlayerController()->bShowMouseCursor;
			}

			UE_LOG(AutomationTestingLog, Display, TEXT("bShowMouseCursor %i"), ProjectTestKey->bShowMouseCursor);

			InputData.Add(ProjectTestKey);
		}
	}
}

UWorld* FAutomationTestingModule::GetWorld() const
{
	return World.Get();
}

void FAutomationTestingModule::SetWorld(UWorld* InWorld)
{
	World = InWorld;
}

bool FAutomationTestingModule::Count()
{
	MixCount++;
	if(MixCount >= 2)
	{
		StopPlay();
		MixCount = 0;
		return true;
	}


	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutomationTestingModule, AutomationTesting)