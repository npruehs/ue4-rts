#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Economy/RTSResourceType.h"

#include "RTSBountyComponent.generated.h"


class AController;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSBountyComponentBountyCollectedSignature, AActor*, Actor,
                                              AController*, Player, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/** Grants players resources when killing the actor. */
UCLASS(meta = (BlueprintSpawnableComponent), Category = "RTS")
class REALTIMESTRATEGY_API URTSBountyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* Event when the bounty was collected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBountyComponentBountyCollectedSignature OnBountyCollected;

protected:
	virtual void BeginPlay() override;

private:
	/** Resources to grant when killing the actor. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TMap<TSubclassOf<URTSResourceType>, float> Bounties;

	UFUNCTION()
	void OnKilled(AActor* Actor, AController* PreviousOwner, AActor* DamageCauser);

	bool GivesBounty(AController* Player);
};
