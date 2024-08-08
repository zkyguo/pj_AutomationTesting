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

void UAutomationTestingBPLibrary::Register(UObject* InObject)
{
	FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
	if(UWorld* InWorld = Cast<UWorld>(InObject))
	{
		InModule.SetWorld(InWorld);
	}
	else if(AActor* InActor = Cast<AActor>(InObject))
	{
		InModule.SetWorld(Cast<UWorld>(InActor->GetWorld()));
	}
}
