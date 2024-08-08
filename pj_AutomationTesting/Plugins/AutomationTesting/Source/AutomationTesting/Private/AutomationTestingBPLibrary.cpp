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

void UAutomationTestingBPLibrary::BuildInputAxial(const FKey& InKey, const FVector2D& Value)
{
	FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
	InModule.BuildInputAxial(InKey, Value);
}

void UAutomationTestingBPLibrary::BuildInput(const FKey& InKey, EInputEvent InputEvent, bool bIsRepeat, float InDelta)
{
	FAutomationTestingModule& InModule = FModuleManager::LoadModuleChecked<FAutomationTestingModule>("AutomationTesting");
	InModule.BuildInput(InKey, InputEvent, FSlateApplication::Get().GetCursorPos(), bIsRepeat, InDelta);
}
