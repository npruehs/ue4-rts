#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"

#include "RTSBuilderComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSBuilderComponentAssignedToConstructionSiteSignature, AActor*, Builder, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSBuilderComponentRemovedFromConstructionSiteSignature, AActor*, Builder, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSBuilderComponentConstructionSiteEnteredSignature, AActor*, Builder, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSBuilderComponentConstructionSiteLeftSignature, AActor*, Builder, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSBuilderComponentConstructionStartedSignature, AActor*, Builder, AActor*, ConstructionSite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSBuilderComponentConstructionFailedSignature, AActor*, Builder, TSubclassOf<AActor>,
    BuildingClass, const FVector&, Location);


/**
 * Allows the actor to construct buildings.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Assigns the builder to the specified construction site. */
	UFUNCTION(BlueprintCallable)
	virtual void AssignToConstructionSite(AActor* ConstructionSite);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	virtual bool BeginConstruction(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Spawns a building of the specified type at the target location and assigns the builder. */
	UFUNCTION(BlueprintCallable)
	bool BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation);

	/** Removes the builder from its assigned construction site. */
	UFUNCTION(BlueprintCallable)
	virtual void LeaveConstructionSite();


    /** Gets the types of buildings the builder can construct. */
    UFUNCTION(BlueprintPure)
    TArray<TSubclassOf<AActor>> GetConstructibleBuildingClasses() const;

    /** Whether the builder enters the construction site while working on it, or not. */
    UFUNCTION(BlueprintPure)
    bool DoesEnterConstructionSite() const;

    /** Gets the distance of the builder to the construction site while building. */
    UFUNCTION(BlueprintPure)
    float GetConstructionSiteOffset() const;

    /** Gets the construction site the builder is currently working on. */
    UFUNCTION(BlueprintPure)
    AActor* GetAssignedConstructionSite() const;

    /** Event when the builder failed to create a new construction site. */
    virtual void NotifyOnConstructionFailed(AActor* Builder, TSubclassOf<AActor> BuildingClass, const FVector& Location);


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
	FRTSBuilderComponentConstructionStartedSignature OnConstructionStarted;

    /** Event when the builder failed to create a new construction site. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FRTSBuilderComponentConstructionFailedSignature OnConstructionFailed;

private:
    /** Types of buildings the builder can construct. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<TSubclassOf<AActor>> ConstructibleBuildingClasses;

    /** Whether the builder enters the construction site while working on it, or not. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bEnterConstructionSite;

    /** Distance of the builder to the construction site while building. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float ConstructionSiteOffset;

	/** Construction site the builder is currently working on. */
    UPROPERTY()
	AActor* AssignedConstructionSite;
};
