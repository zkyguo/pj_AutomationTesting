// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Windows/WindowsApplication.h"

class FAutomationTestingModule : public IModuleInterface, public IWindowsMessageHandler, public FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public :
	void StartRecording();
	void StopRecording();

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult) override;
};
