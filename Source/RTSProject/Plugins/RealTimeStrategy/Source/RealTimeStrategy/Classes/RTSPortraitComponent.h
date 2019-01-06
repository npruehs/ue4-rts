#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSPortraitComponent.generated.h"


class UTexture2D;


/**
* Adds a portrait to the actor that can be shown in the UI.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSPortraitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Portrait of the actor. Can be shown in the UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	UTexture2D* Portrait;
};
