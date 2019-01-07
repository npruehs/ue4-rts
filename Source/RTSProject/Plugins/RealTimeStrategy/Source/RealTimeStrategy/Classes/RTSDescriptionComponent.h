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
    /** Gets the description of the actor. */
    UFUNCTION(BlueprintPure)
    FText GetDescription() const;

private:
    /** Description of the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Description;
};
