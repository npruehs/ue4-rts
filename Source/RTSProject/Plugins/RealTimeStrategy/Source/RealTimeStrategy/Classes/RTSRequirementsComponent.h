#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSRequirementsComponent.generated.h"


class AActor;


/** Adds tech tree requirements to the actor. */
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSRequirementsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Types of actors the player needs to own in order to create this actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    TArray<TSubclassOf<AActor>> RequiredActors;
};
