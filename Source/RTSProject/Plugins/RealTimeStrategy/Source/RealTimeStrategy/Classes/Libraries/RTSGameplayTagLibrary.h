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

    /** Actor can contain builders. */
    static const FGameplayTag& Container_ConstructionSite();

    /** Actor can contain gatherers. */
    static const FGameplayTag& Container_ResourceSource();

    /** Actor is hidden because of being in a container. */
    static const FGameplayTag& HideReason_Container();

    /** Actor can be attacked by other actors. */
    static const FGameplayTag& Status_Permanent_CanBeAttacked();
};
