#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "Economy/RTSGatherData.h"

#include "RTSGathererComponent.generated.h"


class AActor;
class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSGathererComponentResourcesGatheredSignature, AActor*, ResourceSource, const FRTSGatherData&, GatherData, float, GatheredAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSGathererComponentResourcesReturnedSignature, AActor*, ResourceDrain, TSubclassOf<class URTSResourceType>, ResourceType, float, ReturnedAmount);


/**
* Allows the actor to gather resources.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSGathererComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSGathererComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Checks whether the actor can gather from the specified source, e.g. is allowed to gather, and is not at capacity limit. */
	UFUNCTION(BlueprintPure)
	virtual bool CanGatherFrom(AActor* ResourceSource) const;

	/** Finds the closest resource drain for returning currently carried resources. */
	UFUNCTION(BlueprintPure)
	virtual AActor* FindClosestResourceDrain() const;

	/** Gets the resource source the actor has recently been gathering from, if available, or a similar one within its sweep radius. */
	UFUNCTION(BlueprintPure)
	virtual AActor* GetPreferredResourceSource() const;

    /** Gets the closest resource source of the specified type within the passed maximum distance around the actor (0 means anywhere). */
    UFUNCTION(BlueprintPure)
    virtual AActor* GetClosestResourceSource(TSubclassOf<class URTSResourceType> DesiredResourceType, float MaxDistance) const;

	/** Gets the maximum distance for gathering resources from the specified source. */
	UFUNCTION(BlueprintPure)
	virtual float GetGatherRange(AActor* ResourceSource) const;

	/** Whether the gatherer is currently carrying any resources that could be returned. */
	UFUNCTION(BlueprintPure)
	bool IsCarryingResources() const;

	/** Whether this gatherer is currently gathering resources. */
	UFUNCTION(BlueprintPure)
	bool IsGathering() const;

	/** Starts the cooldown timer for gathering resources from the specified source. */
	UFUNCTION(BlueprintCallable)
	virtual void StartGatheringResources(AActor* ResourceSource);

	/** Gathers resources from the specified source and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	virtual float GatherResources(AActor* ResourceSource);

	/** Returns resources to the specified drain. */
	UFUNCTION(BlueprintCallable)
	virtual float ReturnResources(AActor* ResourceDrain);

    /** Gets the before the next resources are gathered, in seconds. */
    UFUNCTION(BlueprintPure)
    float GetRemainingCooldown() const;

	/** Event when the actor has gathered resources from a source. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSGathererComponentResourcesGatheredSignature OnResourcesGathered;

	/** Event when the actor has returned resources to a drain. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSGathererComponentResourcesReturnedSignature OnResourcesReturned;

private:
    /** Resources that can be gathered by the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<FRTSGatherData> GatheredResources;

    /** Types of actors the gatherer can gather resources from. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<TSubclassOf<AActor>> ResourceSourceActorClasses;

    /** Radius in which the actor will automatically gather resources from, in cm. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float ResourceSweepRadius;

    /** Amount of resources the actor is carrying. */
    UPROPERTY(replicated)
    float CarriedResourceAmount;

    /** Type of resource the actor is carrying. */
    UPROPERTY(replicated)
    TSubclassOf<class URTSResourceType> CarriedResourceType;

    /** Resource source the actor is currently gathering from .*/
    UPROPERTY()
    AActor* CurrentResourceSource;

    /** Resource source the actor has been gathering from before.*/
    UPROPERTY()
    AActor* PreviousResourceSource;

    /** Time before the next resources are gathered, in seconds. */
    float RemainingCooldown;

	/** Type of resource gathered before. */
	TSubclassOf<class URTSResourceType> PreviousResourceType;


	bool GetGatherDataForResourceSource(AActor* ResourceSource, FRTSGatherData* OutGatherData) const;
	bool GetGatherDataForResourceType(TSubclassOf<URTSResourceType> ResourceType, FRTSGatherData* OutGatherData) const;
	void LeaveCurrentResourceSource();
};
