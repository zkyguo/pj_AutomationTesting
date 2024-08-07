// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleProjectTestEditor.h"
#include "SimpleProjectTestEditorStyle.h"
#include "SimpleProjectTestEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName SimpleProjectTestEditorTabName("SimpleProjectTestEditor");

#define LOCTEXT_NAMESPACE "FSimpleProjectTestEditorModule"

void FSimpleProjectTestEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleProjectTestEditorStyle::Initialize();
	FSimpleProjectTestEditorStyle::ReloadTextures();

	FSimpleProjectTestEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleProjectTestEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSimpleProjectTestEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimpleProjectTestEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimpleProjectTestEditorTabName, FOnSpawnTab::CreateRaw(this, &FSimpleProjectTestEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSimpleProjectTestEditorTabTitle", "SimpleProjectTestEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSimpleProjectTestEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimpleProjectTestEditorStyle::Shutdown();

	FSimpleProjectTestEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SimpleProjectTestEditorTabName);
}

TSharedRef<SDockTab> FSimpleProjectTestEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSimpleProjectTestEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SimpleProjectTestEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSimpleProjectTestEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SimpleProjectTestEditorTabName);
}

void FSimpleProjectTestEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimpleProjectTestEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimpleProjectTestEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleProjectTestEditorModule, SimpleProjectTestEditor)