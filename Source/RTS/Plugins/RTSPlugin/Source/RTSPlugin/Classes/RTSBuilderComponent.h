#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSBuilderComponent.generated.h"


/**
 * Allows the actor to construct buildings.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Types of buildings the builder can construct. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> ConstructibleBuildingTypes;


	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	void BeginConstruction(TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	void BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation);
};
