// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AutomationTestType.h"
#include "Modules/ModuleManager.h"
#include "Windows/WindowsApplication.h"

class FAutomationTestingModule : public IModuleInterface, public IWindowsMessageHandler, public FTickableGameObject
{
public:
	FAutomationTestingModule();
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void StartRecording();
	void StopRecording();

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult) override;

	//Mouse Action
	bool OnMouseUp(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos);
	bool OnMouseDoubleClick(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos);
	bool OnMouseDown(EMouseButtons::Type MouseButton, const FVector2D& InCursorPos);
	bool OnMouseWheel(const float Delta, const FVector2D& InCursorPos);
	bool OnMouseMove();

	//Keyboard Action
	bool OnKeyUp(const int32 KeyCode, const uint32 CharacterCode, bool bIsRepeat);
	bool OnKeyDown(const int32 KeyCode, const uint32 CharacterCode, bool bIsRepeat);
	bool OnKeyChar(const TCHAR Character, const bool IsRepeat);

protected:
	bool ModifierKeyState[EProjectTestModifierKey::Count];

};
