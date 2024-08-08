// Copyright (C) RenZhai.2024.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"


namespace AutomationProjectTestMethod
{
	AUTOMATIONTESTING_API FKey KeyCodeToFKey(int32 KeyCode);
	AUTOMATIONTESTING_API int32 FKeyToKeyCode(const FKey& Key);
	AUTOMATIONTESTING_API FKey GetFKeyFromCharCode(TCHAR CharCode);
	AUTOMATIONTESTING_API FKey GetMouseKey(EMouseButtons::Type InType);
}