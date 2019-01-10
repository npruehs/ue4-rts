#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSOwnerComponent.generated.h"


class AController;

class ARTSPlayerState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSOwnerComponentOwnerChangedSignature, AController*, NewOwner);


/**
* Specifies the owning player of the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
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


	/** Checks whether the actor belongs to the same team as the specified one. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAsActor(AActor* Other) const;

	/** Checks whether the player owning the actor belongs to the same team as the specified player. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAsController(AController* C) const;


	/** Event when the actor is owned by a different player. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSOwnerComponentOwnerChangedSignature OnOwnerChanged;


private:
	/** Player owning this actor. */
	UPROPERTY(replicated)
	ARTSPlayerState* PlayerOwner;
};
