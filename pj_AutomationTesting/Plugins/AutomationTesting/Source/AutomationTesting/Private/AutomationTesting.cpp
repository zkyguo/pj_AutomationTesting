// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTesting.h"
#include "Windows/WindowsWindow.h"
#include "InputCoreTypes.h"
#include "zlib.h"
#include "Stream/SimpleProjectStream.h"
#include "Misc/ICompressionFormat.h"
#include "Automation/SimpleProjectAutomation.h"
#include "Settings/SimpleProjectTestSettings.h"
#include "Windows/AllowWindowsPlatformTypes.h"


THIRD_PARTY_INCLUDES_START
#include <Windows.h>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "FAutomationTestingModule"

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
}

void FAutomationTestingModule::StopRecording()
{
	if (FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get())
	{
		WindowsApplication->RemoveMessageHandler(*this);
	}

}

TStatId FAutomationTestingModule::GetStatId() const
{
	return TStatId();
}

void FAutomationTestingModule::Tick(float DeltaTime)
{
}

bool FAutomationTestingModule::ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam,
	int32& OutResult)
{
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutomationTestingModule, AutomationTesting)