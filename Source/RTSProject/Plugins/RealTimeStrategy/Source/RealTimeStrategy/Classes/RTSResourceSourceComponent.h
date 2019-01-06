#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSResourceSourceComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSResourceSourceComponentResourcesChangedSignature, float, OldResources, float, NewResources);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSResourceSourceComponentDepletedSignature);


/**
* Adds resources to be gathered from the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSResourceSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Type of resources to be gathered from the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TSubclassOf<URTSResourceType> ResourceType;

	/** Current resources available at the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0), replicated)
	float CurrentResources;

	/** Maximum resources available at the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0))
	float MaximumResources;

	/** Factor to multiply all gathered resources with (e.g. very abundant resource nodes. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float GatheringFactor;

	/** Whether gatherers must enter the resource source for gathering. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bGathererMustEnter;

	/** How many gatherers may enter at the same time. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	int32 GathererCapacity;


	URTSResourceSourceComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;


	/** Extracts resources from this actor, applying gathering factor and checking remaining amount. */
	UFUNCTION(BlueprintCallable)
	virtual float ExtractResources(AActor* Gatherer, float ResourceAmount);

	/** Checks whether the specified gatherer can enter the resource source right now. */
	UFUNCTION(BlueprintCallable)
	virtual bool CanGathererEnter(AActor* Gatherer) const;


	/** Event when the amount of available resources has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceSourceComponentResourcesChangedSignature OnResourcesChanged;

	/** Event when the actor has been depleted. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceSourceComponentDepletedSignature OnDepleted;
};
