#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

#include "RTSConstructionLibrary.generated.h"


/** Utility functions for real-time strategy building construction. */
UCLASS()
class REALTIMESTRATEGY_API URTSConstructionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Gets the index of the specified building class in the list of buildings the passed actor can construct. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static int32 GetConstructableBuildingIndex(AActor* Builder, TSubclassOf<AActor> BuildingClass);

	/** Gets the building class in the list of buildings the passed actor can construct. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static TSubclassOf<AActor> GetConstructableBuildingClass(AActor* Builder, int32 BuildingIndex);
};
