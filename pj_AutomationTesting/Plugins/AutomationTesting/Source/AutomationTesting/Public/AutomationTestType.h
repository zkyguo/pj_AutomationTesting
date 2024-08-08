#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"

struct EProjectTestModifierKey
{
	enum Type
	{
		LeftShift = 0,	// VK_LSHIFT
		RightShift,		// VK_RSHIFT
		LeftControl,	// VK_LCONTROL
		RightControl,	// VK_RCONTROL
		LeftAlt,		// VK_LMENU
		RightAlt,		// VK_RMENU
		CapsLock,		// VK_CAPITAL
		Count,
	};
};

enum class EProjectTestInputType
{
	Input = 0,
	InputAxial,
};

//Action Testing states, which means if current action is Free, Recording or Playing
enum class EProjectTestState
{
	Free = 0,
	Recording,
	Playing,
};

/// Base Struct about action capture Data
struct FProjectTestBaseData :public TSharedFromThis<FProjectTestBaseData>
{
	FProjectTestBaseData()
		:DurationTime(INDEX_NONE)
		, InputType(EProjectTestInputType::Input)
	{}

	float DurationTime;
	FKey Key;
	EProjectTestInputType InputType;
};

/// Use to record Input down and up
struct FProjectTestKey :public FProjectTestBaseData
{
	FProjectTestKey()
		:bIsRepeat(false)
		, MouseButtons(EMouseButtons::Type::Invalid)
	{
		bShowMouseCursor = false;
		InputEvent = EInputEvent::IE_MAX;
		Delta = 0.f;
		FMemory::Memset(ModifierKeyState, 0, (int32)EProjectTestModifierKey::Count + 1);
	}

	bool ModifierKeyState[EProjectTestModifierKey::Count];
	FVector2D CursorPos;
	bool bIsRepeat;
	EInputEvent InputEvent;
	bool bShowMouseCursor;

	EMouseButtons::Type MouseButtons;
	float Delta;
};

/// Use to record Axis 
struct FProjectTestInputAxial :public FProjectTestBaseData
{
	FProjectTestInputAxial()
	{
		DeltaTime = 0.f;
	}

	FVector2D Value;
	float DeltaTime;
};