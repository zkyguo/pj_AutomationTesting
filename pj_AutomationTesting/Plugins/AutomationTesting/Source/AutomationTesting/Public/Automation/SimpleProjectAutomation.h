#pragma once

#include "CoreMinimal.h"
#include "AutomationTestType.h"

namespace ProjectAutomationTest
{
	AUTOMATIONTESTING_API void StartAutomationTest(
		const TArray<TSharedPtr<FProjectTestBaseData>>& InputDatas,
		const TArray<TSharedPtr<FProjectTestBaseData>>& AxialInputData);

	AUTOMATIONTESTING_API void SimulateKeyPress(FKey key, EInputEvent InputEvent);
	AUTOMATIONTESTING_API void SimulateKeyPress(FKey key, const FVector2D& InValue, float DeltaTime);
	

	AUTOMATIONTESTING_API void SetMouse(const FVector2D& InPos);
	AUTOMATIONTESTING_API void ShowMouseCursor(bool bIsShow);
}