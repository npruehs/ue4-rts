#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"

#include "RTSGameplayTagLibrary.generated.h"

/**
* Utility functions for adding, checking and removing gameplay tags.
*/
UCLASS()
class REALTIMESTRATEGY_API URTSGameplayTagLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
    UFUNCTION(BlueprintPure, Category = "RTS")
    static bool HasGameplayTag(const AActor* Actor, const FGameplayTag& Tag);

    /** Actor can be attacked by other actors. */
    static const FGameplayTag& Status_Permanent_CanBeAttacked();
};
