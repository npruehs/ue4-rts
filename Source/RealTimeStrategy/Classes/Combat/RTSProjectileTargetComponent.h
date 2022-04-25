#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSProjectileTargetComponent.generated.h"


class UMeshComponent;


/** Specifies valid locations to fire projectiles at. */
UCLASS(ClassGroup="RTS", Category = "RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSProjectileTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** Gets a random world location to fire a projectile at. */
	UFUNCTION(BlueprintPure)
	FVector GetRandomProjectileTargetLocation() const;

private:
	/** Sockets to fire projectiles at. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<FName> TargetSockets;

	/** Component tag of the mesh component to look for sockets at. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FName PrimaryMeshComponentTag;

	UPROPERTY()
	UMeshComponent* MeshComponent;
};
