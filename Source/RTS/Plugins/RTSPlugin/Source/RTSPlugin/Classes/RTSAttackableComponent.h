#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSAttackableComponent.generated.h"


/**
* Allows choosing the actor as target for an attack order.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSAttackableComponent : public UActorComponent
{
	GENERATED_BODY()
};
