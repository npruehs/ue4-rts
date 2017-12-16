#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSProductionCostType.h"

#include "RTSProductionCostComponent.generated.h"


class URTSResourceType;


/** Specifies the time and resources required to construct the actor. */
UCLASS(meta = (BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSProductionCostComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** When to pay resources for production. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	ERTSProductionCostType ProductionCostType;

	/** Time for producing the actor, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ProductionTime;

	/** Resources to pay for producing the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TMap<TSubclassOf<URTSResourceType>, float> Resources;

	/** Resources to refund when canceling production. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float RefundFactor;


	URTSProductionCostComponent(const FObjectInitializer& ObjectInitializer);
};
