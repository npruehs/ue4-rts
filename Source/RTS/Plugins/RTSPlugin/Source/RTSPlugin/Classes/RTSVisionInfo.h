#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Info.h"

#include "RTSVisionState.h"

#include "RTSVisionInfo.generated.h"


class ARTSVisionVolume;


/**
* Defines the visible areas for a player or team.
*/
UCLASS()
class ARTSVisionInfo : public AInfo
{
	GENERATED_BODY()

	ARTSVisionInfo();

public:
	/** Index of the team this actor tracks the vision of. */
	uint8 TeamIndex;

	/** Which tiles are currently unknown, known and visible. */
	TArray<ERTSVisionState> Tiles;


	virtual void Tick(float DeltaSeconds) override;


protected:
	virtual void BeginPlay() override;

	
private:
	ARTSVisionVolume* VisionVolume;


	int32 GetTileIndex(int X, int Y);
};
