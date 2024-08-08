// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTestingBPLibrary.h"
#include "AutomationTesting.h"

UAutomationTestingBPLibrary::UAutomationTestingBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UAutomationTestingBPLibrary::StartRecording()
{
	FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
	InModule.StartRecording();

	return true;
}

bool UAutomationTestingBPLibrary::StopRecording(const FString& InSavePath)
{
	FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
	InModule.StopRecording();

	return false;
}
