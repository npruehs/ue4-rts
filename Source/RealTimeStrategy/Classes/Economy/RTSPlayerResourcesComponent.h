#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"

#include "Economy/RTSResourceType.h"

#include "RTSPlayerResourcesComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSPlayerResourcesComponentResourcesChangedSignature, TSubclassOf<URTSResourceType>, ResourceType, float, OldResourceAmount, float, NewResourceAmount, bool, bSyncedFromServer);


/**
* Attach to player or AI controllers. Allows the controller to store resources.
*/
UCLASS(ClassGroup="RTS", Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSPlayerResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSPlayerResourcesComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;


	/** Gets the amount of resources in stock of this player. */
	UFUNCTION(BlueprintPure)
	float GetResources(TSubclassOf<URTSResourceType> ResourceType) const;

	/** Gets the types of resources available to this player. */
	TArray<TSubclassOf<URTSResourceType>> GetResourceTypes() const;

	/** Checks the amount of resources in stock of this player. */
	UFUNCTION(BlueprintPure)
	bool CanPayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount) const;

	/** Checks the amount of resources in stock of this player. */
	UFUNCTION(BlueprintPure)
	bool CanPayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources) const;

	/** Adds the specified resources to the stock of this player. */
	UFUNCTION(BlueprintCallable)
	virtual float AddResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

	/** Removes the specified resources from the stock of this player. */
	UFUNCTION(BlueprintCallable)
	virtual float PayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

	/** Removes the specified resources from the stock of this player. */
	UFUNCTION(BlueprintCallable)
	virtual void PayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources);


	/** Event when the current resource stock amount for the player has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSPlayerResourcesComponentResourcesChangedSignature OnResourcesChanged;

private:
	/** Types of resources currently available to this player. Num must match ResourceAmounts. Need to use an array here instead of map for replication. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Resources")
	TArray<TSubclassOf<URTSResourceType>> ResourceTypes;

	/** Resources currently available to this player. Num must match ResourceTypes. Need to use an array here instead of map for replication. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Resources", ReplicatedUsing = ReceivedResourceAmounts)
	TArray<float> ResourceAmounts;

	UFUNCTION()
	void ReceivedResourceAmounts();
};
