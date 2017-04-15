#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSSelectableComponent.generated.h"


/**
 * Allows selecting the actor, e.g. by left-click.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()
};
