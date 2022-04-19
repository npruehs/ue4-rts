#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Volume.h"

#include "RTSMinimapVolume.generated.h"


class UTexture2D;


/**
* Volume that defines the size and resolution of the minimap.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API ARTSMinimapVolume : public AVolume
{
	GENERATED_BODY()

public:
	/** Gets the Background image of the minimap. */
	UFUNCTION(BlueprintPure)
	UTexture2D* GetMinimapImage() const;

private:
	/** Background image of the minimap. */
	UPROPERTY(EditInstanceOnly, Category = "RTS")
	UTexture2D* MinimapImage;
};
