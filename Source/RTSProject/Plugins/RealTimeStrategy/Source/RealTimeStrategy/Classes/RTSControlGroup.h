#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Actor.h"

#include "RTSControlGroup.generated.h"

/** Group of actors. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSControlGroup
{
    GENERATED_USTRUCT_BODY()

public:
    /** Actors of this control group. */
    UPROPERTY()
    TArray<AActor*> Actors;
};
