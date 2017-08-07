#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSBuilderComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSBuilderComponentAssignedToConstructionSiteSignature, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSBuilderComponentRemovedFromConstructionSiteSignature, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSBuilderComponentConstructionSiteEnteredSignature, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSBuilderComponentConstructionSiteLeftSignature, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSBuilderComponentConstructionStartedSignature, AActor*, ConstructionSite);


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
	virtual void AssignToConstructionSite(AActor* ConstructionSite);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	virtual void BeginConstruction(TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	void BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation);

	/** Gets the construction site the builder is currently working on. */
	UFUNCTION(BlueprintCallable)
	AActor* GetAssignedConstructionSite();

	/** Removes the builder from its assigned construction site. */
	UFUNCTION(BlueprintCallable)
	virtual void LeaveConstructionSite();


	/** Event when the builder has been assigned to a construction site. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBuilderComponentAssignedToConstructionSiteSignature OnAssignedToConstructionSite;

	/** Event when the builder is no longer assigned to a construction site. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBuilderComponentRemovedFromConstructionSiteSignature OnRemovedFromConstructionSite;

	/** Event when the builder has entered a construction site. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBuilderComponentConstructionSiteEnteredSignature OnConstructionSiteEntered;

	/** Event when the builder has left a construction site. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBuilderComponentConstructionSiteLeftSignature OnConstructionSiteLeft;

	/** Event when the builder has created a new construction site. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSBuilderComponentConstructionSiteEnteredSignature OnConstructionStarted;


private:
	/** Construction site the builder is currently working on. */
	AActor* AssignedConstructionSite;
};
