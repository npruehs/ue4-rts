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
	URTSResourceSourceComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;


	/** Extracts resources from this actor, applying gathering factor and checking remaining amount. */
	UFUNCTION(BlueprintCallable)
	virtual float ExtractResources(AActor* Gatherer, float ResourceAmount);

	/** Checks whether the specified gatherer can enter the resource source right now. */
	UFUNCTION(BlueprintPure)
	virtual bool CanGathererEnter(AActor* Gatherer) const;


    /** Gets type of resources to be gathered from the actor. */
    UFUNCTION(BlueprintPure)
    TSubclassOf<URTSResourceType> GetResourceType() const;

    /** Gets the maximum resources available at the actor. */
    UFUNCTION(BlueprintPure)
    float GetMaximumResources() const;

    /** Gets the factor to multiply all gathered resources with (e.g. very abundant resource nodes. */
    UFUNCTION(BlueprintPure)
    float GetGatheringFactor() const;

    /** Whether gatherers must enter the resource source for gathering. */
    UFUNCTION(BlueprintPure)
    bool MustGathererEnter() const;

    /** Gets how many gatherers may enter at the same time. */
    UFUNCTION(BlueprintPure)
    int32 GetGathererCapacity() const;

    /** Gets the current resources available at the actor. */
    UFUNCTION(BlueprintPure)
    float GetCurrentResources() const;


	/** Event when the amount of available resources has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceSourceComponentResourcesChangedSignature OnResourcesChanged;

	/** Event when the actor has been depleted. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceSourceComponentDepletedSignature OnDepleted;

private:
    /** Type of resources to be gathered from the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TSubclassOf<URTSResourceType> ResourceType;

    /** Maximum resources available at the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float MaximumResources;

    /** Factor to multiply all gathered resources with (e.g. very abundant resource nodes. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float GatheringFactor;

    /** Whether gatherers must enter the resource source for gathering. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bGathererMustEnter;

    /** How many gatherers may enter at the same time. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    int32 GathererCapacity;

    /** Current resources available at the actor. */
    UPROPERTY(replicated)
    float CurrentResources;
};
