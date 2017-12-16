#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/Volume.h"

#include "RTSVisionVolume.generated.h"


/**
* Volume that defines the size and resolution of vision provided by units.
*/
UCLASS(Blueprintable)
class ARTSVisionVolume : public AVolume
{
	GENERATED_UCLASS_BODY()

public:
	/** Size of a single tile of the vision grid imposed on the world. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0))
	float SizePerTile;


	FVector GetWorldSize() const;
	FIntVector GetTileSize() const;

	FIntVector WorldToTile(const FVector& WorldPosition) const;

protected:
	virtual void BeginPlay() override;

private:
	FVector WorldSize;
	FIntVector TileSize;
};
