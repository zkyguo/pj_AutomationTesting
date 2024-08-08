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