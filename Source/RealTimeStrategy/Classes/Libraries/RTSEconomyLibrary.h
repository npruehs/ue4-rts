#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"

#include "Economy/RTSResourceType.h"

#include "RTSEconomyLibrary.generated.h"


/**
* Utility functions for real-time strategy economy.
*/
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSEconomyLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Gets the display name of the specified resource. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static FText GetResourceName(TSubclassOf<URTSResourceType> ResourceClass);

	/** Gets the icon to show next to resource amounts in the UI. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static UTexture2D* GetResourceIcon(TSubclassOf<URTSResourceType> ResourceClass);

	/** Gets the color to use when showing resource amounts in the UI. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static FLinearColor GetResourceColor(TSubclassOf<URTSResourceType> ResourceClass);
};
