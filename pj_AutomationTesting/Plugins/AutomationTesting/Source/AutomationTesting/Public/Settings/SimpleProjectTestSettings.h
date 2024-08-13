#pragma once

#include "CoreMinimal.h"
#include "SimpleProjectTestSettings.generated.h"

UCLASS(config = SimpleProjectTestSettings)
class AUTOMATIONTESTING_API USimpleProjectTestSettings : public UObject
{
	GENERATED_BODY()

public:
	USimpleProjectTestSettings();

	UPROPERTY(config, EditAnywhere, Category = CustomSettings, meta = (ToolTip = "Custom."))
	bool bCustomInput;

	UPROPERTY(config, EditAnywhere, Category = CustomSettings, meta = (ToolTip = "Custom"))
	bool bCustomAxialInput;
};