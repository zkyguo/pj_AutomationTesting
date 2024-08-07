// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleProjectTestEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleProjectTestEditorModule"

void FSimpleProjectTestEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SimpleProjectTestEditor", "Bring up SimpleProjectTestEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
