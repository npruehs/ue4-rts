#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSPlayerResourcesComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSPlayerResourcesComponentResourcesChangedSignature, TSubclassOf<URTSResourceType>, ResourceType, float, OldResourceAmount, float, NewResourceAmount, bool, bSyncedFromServer);


/**
* Attach to player or AI controllers. Allows the controller to store resources.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSPlayerResourcesComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URTSPlayerResourcesComponent(const FObjectInitializer& ObjectInitializer);

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void BeginPlay() override;


    /** Resources currently available to this player. Num must match ResourceTypes. Need to use an array here instead of map for replication. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Resources", ReplicatedUsing = ReceivedResourceAmounts)
    TArray<float> ResourceAmounts;

    /** Types of resources currently available to this player. Num must match ResourceAmounts. Need to use an array here instead of map for replication. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Resources", replicated)
    TArray<TSubclassOf<URTSResourceType>> ResourceTypes;


    /** Gets the amount of resources in stock of this player. */
    bool GetResources(TSubclassOf<URTSResourceType> ResourceType, float* OutResourceAmount);

    /** Checks the amount of resources in stock of this player. */
    bool CanPayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

    /** Checks the amount of resources in stock of this player. */
    bool CanPayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources);

    /** Adds the specified resources to the stock of this player. */
    virtual float AddResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

    /** Removes the specified resources from the stock of this player. */
    virtual float PayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount);

    /** Removes the specified resources from the stock of this player. */
    virtual void PayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources);


    /** Event when the current resource stock amount for the player has changed. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FRTSPlayerResourcesComponentResourcesChangedSignature OnResourcesChanged;

private:
    UFUNCTION()
    void ReceivedResourceAmounts();
};
