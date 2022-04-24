#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSOwnerComponent.generated.h"


class AController;

class ARTSPlayerState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSOwnerComponentOwnerChangedSignature, AActor*, Actor, AController*, NewOwner);


/**
* Specifies the owning player of the actor.
*/
UCLASS(Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSOwnerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** Gets the player owning the actor. */
	UFUNCTION(BlueprintPure)
	ARTSPlayerState* GetPlayerOwner() const;

	/** Sets the player owning the actor. */
	UFUNCTION(BlueprintCallable)
	void SetPlayerOwner(AController* Controller);

	/** Sets the player owning the actor. */
	UFUNCTION(BlueprintCallable)
	void SetPlayerStateOwner(ARTSPlayerState* PlayerState);


	/** Checks whether the actor belongs to the same team as the specified one. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAsActor(AActor* Other) const;

	/** Checks whether the player owning the actor belongs to the same team as the specified player. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAsController(AController* C) const;

	/** Gets the index of the player that should initially own the actor. */
	uint8 GetInitialOwnerPlayerIndex();


	/** Event when the actor is owned by a different player. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSOwnerComponentOwnerChangedSignature OnOwnerChanged;


private:
	/** Index of the player that should initially own the actor. */
	UPROPERTY(EditInstanceOnly, Category = "RTS")
	uint8 InitialOwnerPlayerIndex;

	/** Player owning this actor. */
	UPROPERTY(ReplicatedUsing=ReceivedPlayerOwner)
	ARTSPlayerState* PlayerOwner;


	UFUNCTION()
	void ReceivedPlayerOwner(ARTSPlayerState* PreviousOwner);

	void NotifyOnOwnerChanged(ARTSPlayerState* PreviousOwner, ARTSPlayerState* NewOwner);
};
