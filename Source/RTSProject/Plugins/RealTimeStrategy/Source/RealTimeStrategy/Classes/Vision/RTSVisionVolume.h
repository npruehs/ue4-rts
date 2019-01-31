#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Volume.h"

#include "RTSVisionVolume.generated.h"


/**
* Volume that defines the size and resolution of vision provided by units.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API ARTSVisionVolume : public AVolume
{
	GENERATED_BODY()

public:
    ARTSVisionVolume(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Gets the width and height of the vision grid imposed on the world. */
    int32 GetSizeInTiles() const;

    /** Gets the size of the vision volume, in world space. */
	FVector GetSizeInWorld() const;

    /** Gets the width and height of a single vision grid tile, in cm. */
    float GetTileSize() const;
    
	FIntVector WorldToTile(const FVector& WorldPosition) const;

protected:
	virtual void BeginPlay() override;

private:
    /** Width and height of the vision grid imposed on the world. */
    UPROPERTY(EditInstanceOnly, Category = "RTS")
    int32 SizeInTiles;

    /** Size of the vision volume, in world space. */
    FVector SizeInWorld;

    /** Width and height of a single vision grid tile, in cm. */
    float TileSize;
};
