#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSResourceDrainComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSResourceDrainComponentResourcesReturnedSignature, AActor*, Gatherer, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/**
* Allows resources to be returned to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSResourceDrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Types of resources that can be returned to the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<URTSResourceType>> ResourceTypes;

	/** Whether gatherers must enter the resource drain for returning resources. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bGathererMustEnter;

	/** How many gatherers may enter at the same time. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	int32 GathererCapacity;


	URTSResourceDrainComponent(const FObjectInitializer& ObjectInitializer);


	/** Returns resources to this actor, notifying the owning player. */
	virtual float ReturnResources(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);


	/** Event when resources have been returned to the actor. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSResourceDrainComponentResourcesReturnedSignature OnResourcesReturned;
};
