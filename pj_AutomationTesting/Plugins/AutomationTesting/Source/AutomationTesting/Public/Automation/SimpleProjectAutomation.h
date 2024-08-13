#pragma once

#include "CoreMinimal.h"
#include "AutomationTestType.h"

namespace ProjectAutomationTest
{
	AUTOMATIONTESTING_API void SetMouse(const FVector2D& InPos);
	AUTOMATIONTESTING_API void ShowMouseCursor(bool bIsShow);
}