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

	/** Whether the builder enters the construction site while working on it, or not. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bEnterConstructionSite;


	/** Assigns the builder to the specified construction site. */
	UFUNCTION(BlueprintCallable)
	void AssignToConstructionSite(AActor* ConstructionSite);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	void BeginConstruction(TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	void BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation);

	/** Gets the construction site the builder is currently working on. */
	UFUNCTION(BlueprintCallable)
	AActor* GetAssignedConstructionSite();

	/** Removes the builder from its assigned construction site. */
	UFUNCTION(BlueprintCallable)
	void LeaveConstructionSite();

private:
	/** Construction site the builder is currently working on. */
	AActor* AssignedConstructionSite;
};
