#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSGatherData.h"

#include "RTSGathererComponent.generated.h"


class AActor;
class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSGathererComponentResourcesGatheredSignature, AActor*, ResourceSource, const FRTSGatherData&, GatherData, float, GatheredAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSGathererComponentResourcesReturnedSignature, AActor*, ResourceDrain, TSubclassOf<class URTSResourceType>, ResourceType, float, ReturnedAmount);


/**
* Allows the actor to gather resources.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSGathererComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Amount of resources the actor is carrying. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS", replicated)
	float CarriedResourceAmount;

	/** Type of resource the actor is carrying. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS", replicated)
	TSubclassOf<class URTSResourceType> CarriedResourceType;

	/** Resources that can be gathered by the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<FRTSGatherData> GatheredResources;

	/** Time before the next resources are gathered, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float RemainingCooldown;

	/** Types of actors the gatherer can gather resources from. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> ResourceSourceActorClasses;

	/** Radius in which the actor will automatically gather resources from, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ResourceSweepRadius;


	URTSGathererComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Checks whether the actor can gather from the specified source, e.g. is allowed to gather, and is not at capacity limit. */
	UFUNCTION(BlueprintCallable)
	virtual bool CanGatherFrom(AActor* ResourceSource);

	/** Finds the closest resource drain for returning currently carried resources. */
	UFUNCTION(BlueprintCallable)
	virtual AActor* FindClosestResourceDrain() const;

	/** Gets the resource source the actor has recently been gathering from, if available, or a similar one within its sweep radius. */
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetPreferredResourceSource() const;

	/** Gets the maximum distance for gathering resources from the specified source. */
	UFUNCTION(BlueprintCallable)
	virtual float GetGatherRange(AActor* ResourceSource);

	/** Whether the gatherer is currently carrying any resources that could be returned. */
	UFUNCTION(BlueprintCallable)
	bool IsCarryingResources() const;

	/** Whether this gatherer is currently gathering resources. */
	UFUNCTION(BlueprintCallable)
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


	/** Event when the actor has gathered resources from a source. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSGathererComponentResourcesGatheredSignature OnResourcesGathered;

	/** Event when the actor has returned resources to a drain. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSGathererComponentResourcesReturnedSignature OnResourcesReturned;

private:
	/** Resource source the actor is currently gathering from .*/
	AActor* CurrentResourceSource;

	/** Resource source the actor has been gathering from before.*/
	AActor* PreviousResourceSource;

	/** Type of resource gathered before. */
	TSubclassOf<class URTSResourceType> PreviousResourceType;


	bool GetGatherDataForResourceSource(AActor* ResourceSource, FRTSGatherData* OutGatherData);
	bool GetGatherDataForResourceType(TSubclassOf<URTSResourceType> ResourceType, FRTSGatherData* OutGatherData);
	void LeaveCurrentResourceSource();
};
