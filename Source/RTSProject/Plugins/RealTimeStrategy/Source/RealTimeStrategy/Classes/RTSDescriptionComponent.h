#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSDescriptionComponent.generated.h"


/**
* Adds a localizable description to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSDescriptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Description of the actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FText Description;
};
