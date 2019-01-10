#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"

#include "Economy/RTSPaymentType.h"
#include "Economy/RTSResourceType.h"

#include "RTSProductionCostComponent.generated.h"


/** Specifies the time and resources required to construct the actor. */
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSProductionCostComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    URTSProductionCostComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    /** When to pay resources for production. */
    UFUNCTION(BlueprintPure)
    ERTSPaymentType GetProductionCostType() const;

    /** Gets the tme for producing the actor, in seconds. */
    UFUNCTION(BlueprintPure)
    float GetProductionTime() const;

    /** Gets the resources to pay for producing the actor. */
    UFUNCTION(BlueprintPure)
    TMap<TSubclassOf<URTSResourceType>, float> GetResources() const;

    /** Gets the resources to refund when canceling production. */
    UFUNCTION(BlueprintPure)
    float GetRefundFactor() const;


private:
	/** When to pay resources for production. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	ERTSPaymentType ProductionCostType;

	/** Time for producing the actor, in seconds. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
	float ProductionTime;

	/** Resources to pay for producing the actor. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TMap<TSubclassOf<URTSResourceType>, float> Resources;

	/** Resources to refund when canceling production. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	float RefundFactor;
};
