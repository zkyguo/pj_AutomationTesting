// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleProjectTestEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSimpleProjectTestEditorStyle::StyleInstance = nullptr;

void FSimpleProjectTestEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSimpleProjectTestEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSimpleProjectTestEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SimpleProjectTestEditorStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FSimpleProjectTestEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SimpleProjectTestEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("AutomationTesting")->GetBaseDir() / TEXT("Resources"));

	Style->Set("SimpleProjectTestEditor.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FSimpleProjectTestEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSimpleProjectTestEditorStyle::Get()
{
	return *StyleInstance;
}
