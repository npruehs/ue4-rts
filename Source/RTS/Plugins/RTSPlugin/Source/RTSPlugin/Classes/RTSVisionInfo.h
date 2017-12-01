#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Info.h"

#include "RTSVisionState.h"

#include "RTSVisionInfo.generated.h"


class UWorld;

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


	virtual void Tick(float DeltaSeconds) override;

	/** Gets the state of the tile with the specified coordinates. */
	ERTSVisionState GetVision(int32 X, int32 Y) const;
	
	/** Gets vision information for the local player. */
	static ARTSVisionInfo* GetLocalVisionInfo(UWorld* World);

protected:
	virtual void BeginPlay() override;

	
private:
	ARTSVisionVolume* VisionVolume;

	/** Which tiles are currently unknown, known and visible. */
	TArray<ERTSVisionState> Tiles;

	bool GetTileCoordinates(int Index, int* OutX, int* OutY) const;
	int32 GetTileIndex(int X, int Y) const;
};
