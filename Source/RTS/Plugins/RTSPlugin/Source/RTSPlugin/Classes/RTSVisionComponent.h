#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSVisionComponent.generated.h"


/**
* Allows the actor to reveal areas covered by fog of war.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Radius in which the actor reveals areas covered by fog of war, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0), replicated)
	float SightRadius;


	URTSVisionComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
