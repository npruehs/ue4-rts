#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSResourceDrainComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSResourceDrainComponentResourcesReturnedSignature, AActor*, Gatherer, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/**
* Allows resources to be returned to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSResourceDrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSResourceDrainComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/** Returns resources to this actor, notifying the owning player. */
	virtual float ReturnResources(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

    
    /** Gets the types of resources that can be returned to the actor. */
    UFUNCTION(BlueprintPure)
    TArray<TSubclassOf<URTSResourceType>> GetResourceTypes() const;

    /** Whether gatherers must enter the resource drain for returning resources. */
    UFUNCTION(BlueprintPure)
    bool MustGathererEnter() const;

    /** Gets how many gatherers may enter at the same time. */
    UFUNCTION(BlueprintPure)
    int32 GetGathererCapacity() const;


    /** Event when resources have been returned to the actor. */
    UFUNCTION(NetMulticast, reliable)
    virtual void NotifyOnResourcesReturned(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);


	/** Event when resources have been returned to the actor. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceDrainComponentResourcesReturnedSignature OnResourcesReturned;

private:
    /** Types of resources that can be returned to the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<TSubclassOf<URTSResourceType>> ResourceTypes;

    /** Whether gatherers must enter the resource drain for returning resources. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bGathererMustEnter;

    /** How many gatherers may enter at the same time. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 1))
    int32 GathererCapacity;
};
