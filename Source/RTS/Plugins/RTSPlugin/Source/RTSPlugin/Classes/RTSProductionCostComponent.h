#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSProductionCostComponent.generated.h"


/** Specifies the time and resources required to construct the actor. */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSProductionCostComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Time for producing the actor, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ProductionTime;
};
