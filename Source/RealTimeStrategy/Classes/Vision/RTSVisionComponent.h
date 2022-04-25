#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Vision/RTSVisionInfoTileList.h"

#include "RTSVisionComponent.generated.h"


/**
* Allows the actor to reveal areas covered by fog of war.
*/
UCLASS(ClassGroup="RTS", Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSVisionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Performance Optimization. Tile the actor has been updated its vision at most recently. */
	FIntVector ActorLocationTile;

	/** Performance Optimization. Remembers the tiles we've got vision at. */
	UPROPERTY()
	TArray<FRTSVisionInfoTileList> VisibleTiles;

	/** Gets the radius in which the actor reveals areas covered by fog of war, in cm. */
	UFUNCTION(BlueprintPure)
	float GetSightRadius() const;

	/** Whether the actor can see tiles above its own height level. */
	bool IgnoresHeightLevels() const;

private:
	/** Radius in which the actor reveals areas covered by fog of war, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
	float SightRadius;

	/** Whether the actor can see tiles above its own height level. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	bool bIgnoreHeightLevels;
};
