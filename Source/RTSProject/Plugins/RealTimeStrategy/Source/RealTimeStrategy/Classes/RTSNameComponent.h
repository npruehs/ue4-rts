#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSNameComponent.generated.h"


/**
* Adds a localizable name to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSNameComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Name of the actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FText Name;
};
