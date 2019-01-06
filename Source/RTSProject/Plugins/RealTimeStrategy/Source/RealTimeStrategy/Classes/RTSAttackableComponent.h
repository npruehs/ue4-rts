#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSAttackableComponent.generated.h"


/**
* Allows choosing the actor as target for an attack order.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSAttackableComponent : public UActorComponent
{
	GENERATED_BODY()
};
