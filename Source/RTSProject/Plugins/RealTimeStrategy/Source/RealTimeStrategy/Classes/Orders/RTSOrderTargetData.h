#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "RTSOrderTargetData.generated.h"


class AActor;


/** Target data (actor, location) for an order. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSOrderTargetData
{
    GENERATED_BODY()

public:
    /** Target actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    AActor* Actor;

    /** Target location. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FVector2D Location;

    /** Gameplay tags of the target. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FGameplayTagContainer TargetTags;
};
