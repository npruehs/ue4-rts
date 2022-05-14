#pragma once

#include "CoreMinimal.h"

#include "RHI.h"
#include "GameFramework/Actor.h"
#include "NavFilters/NavigationQueryFilter.h"

#include "UI/RTSRangeIndicator.h"

#include "RTSBuildingCursor.generated.h"


class USkeletalMesh;
class UStaticMesh;
class UTexture2D;


/** 3D cursor for selecting a location for a building. */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API ARTSBuildingCursor : public AActor
{
	GENERATED_BODY()

public:
	ARTSBuildingCursor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;


	/** Initializes this building cursor up for the specified building class. */
	void SetupForBuilding(TSubclassOf<AActor> BuildingClass);

	/** Gets a texture whose red pixels highlight blocked grid cells. */
	UFUNCTION(BlueprintPure)
	UTexture2D* GetGridTexture() const;

	/** Gets the width and height of the grid, in cells. */
	UFUNCTION(BlueprintPure)
	int32 GetGridWidthAndHeight() const;

	/** Sets the width and height of the grid, in cells. */
	UFUNCTION(BlueprintCallable)
	void SetGridWidthAndHeight(int32 InGridWidthAndHeight);

	/** Gets the size of a grid cell, in cm. */
	UFUNCTION(BlueprintPure)
	float GetGridCellSize() const;

	/** Whether this building cursor is currently showing a grid. */
	UFUNCTION(BlueprintPure)
	bool HasGrid() const;

	/** Checks whether all grid cells are valid with respect to all checks. */
	UFUNCTION(BlueprintPure)
	bool AreAllCellsValid() const;

	/** Updates the location of this cursor, snapping to the grid if required. */
	void SetCursorLocation(const FVector& NewWorldLocation);

	/** Sets the range to preview while this cursor is shown. */
	void SetRange(float Range);


	/** Sets the preview skeletal mesh of this cursor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void SetSkeletalMesh(USkeletalMesh* BuildingMesh, const FTransform& RelativeTransform);

	/** Sets the preview static mesh of this cursor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void SetStaticMesh(UStaticMesh* BuildingMesh, const FTransform& RelativeTransform);

	/** Applies the visuals for either a valid or an invalid location. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void SetLocationValid(bool bValid);


private:
	/** Size of a grid cell, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Grid")
	float GridCellSize;

	/** Whether to check collision for each grid cell. */
	UPROPERTY()
	bool bCheckCollision;

	/** Collision object types to detect obstacles that block the placement of a building. */
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> BuildingLocationDetectionChannels;

	/** Whether to check navigation for each grid cell. You should enable Allow Client Side Navigation in your project settings when using this. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Grid")
	bool bCheckNavigation;

	/** Navigation filter to use for checking navigation of the grid cells. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Grid", meta = (EditCondition = "bCheckNavigation"))
	TSubclassOf<UNavigationQueryFilter> NavigationQueryFilterClass;

	/** How for to check for valid nearby navigation areas around the center of each grid cell. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Grid", meta = (EditCondition = "bCheckNavigation"))
	FVector NavigationQueryExtent;

	/** Preview to use for attack ranges. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Range")
	TSubclassOf<ARTSRangeIndicator> RangeIndicatorClass;

	/** Width and height of the grid, in cells. */
	int32 GridWidthAndHeight;

	/** Whether all grid cells are valid with respect to all checks. */
	bool bAllCellsValid;

	/** Buffer for updating the texture whose pixels highlight blocked grid cells. */
	uint8* GridTextureBuffer;

	/** Texture whose pixels highlight blocked grid cells. */
	UPROPERTY()
	UTexture2D* GridTexture;

	/** Update region of the texture whose pixels highlight blocked grid cells. */
	FUpdateTextureRegion2D GridUpdateTextureRegion;

	UPROPERTY()
	ARTSRangeIndicator* RangeIndicator;

	/** Checks whether the player can construct buildings at the tile representing the specified world location. */
	bool CanConstructBuildingAt(const FVector& WorldLocation);
};
