// Copyright Epic Games, Inc. All Rights Reserved.

#include "pj_AutomationTestingGameMode.h"
#include "pj_AutomationTestingCharacter.h"
#include "UObject/ConstructorHelpers.h"

Apj_AutomationTestingGameMode::Apj_AutomationTestingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
