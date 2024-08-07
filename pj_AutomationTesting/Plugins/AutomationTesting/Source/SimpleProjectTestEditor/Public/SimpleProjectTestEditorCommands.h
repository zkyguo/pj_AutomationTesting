// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimpleProjectTestEditorStyle.h"

class FSimpleProjectTestEditorCommands : public TCommands<FSimpleProjectTestEditorCommands>
{
public:

	FSimpleProjectTestEditorCommands()
		: TCommands<FSimpleProjectTestEditorCommands>(TEXT("SimpleProjectTestEditor"), NSLOCTEXT("Contexts", "SimpleProjectTestEditor", "SimpleProjectTestEditor Plugin"), NAME_None, FSimpleProjectTestEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};