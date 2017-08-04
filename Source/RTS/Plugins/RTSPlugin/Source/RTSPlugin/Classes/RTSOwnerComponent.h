#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSOwnerComponent.generated.h"


class AController;

class ARTSPlayerState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSOwnerComponentOwnerChangedSignature, AController*, NewOwner);


/**
* Specifies the owning player of the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URTSOwnerComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** Gets the player owning this character. */
	UFUNCTION(BlueprintCallable)
	ARTSPlayerState* GetPlayerOwner();

	/** Sets the player owning this character. */
	UFUNCTION(BlueprintCallable)
	void SetPlayerOwner(AController* Controller);


	/** Checks whether this character belongs to the same team as the specified one. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAsCharacter(ARTSCharacter* Other);

	/** Checks whether the player owning this character belongs to the same team as the specified player. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAsController(AController* C);


	/** Event when the character is owned by a different player. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSOwnerComponentOwnerChangedSignature OnOwnerChanged;


private:
	/** Player owning this character. */
	UPROPERTY(replicated)
	ARTSPlayerState* PlayerOwner;
};
