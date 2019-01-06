#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Info.h"

#include "RTSVisionState.h"

#include "RTSVisionInfo.generated.h"


class UWorld;

class ARTSVisionVolume;


/**
* Defines the visible areas for a player or team.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSVisionInfo : public AInfo
{
	GENERATED_BODY()

	ARTSVisionInfo();

public:
	virtual void Tick(float DeltaSeconds) override;

	/** Index of the team this actor keeps track of the vision for. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = ReceivedTeamIndex, Category = "Team")
	uint8 TeamIndex;

	/** Sets the index of the team this actor keeps track of the vision for. */
	void SetTeamIndex(uint8 NewTeamIndex);

	/** Gets the state of the tile with the specified coordinates. */
	ERTSVisionState GetVision(int32 X, int32 Y) const;
	
	/** Gets vision information for the specified team. */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static ARTSVisionInfo* GetVisionInfoForTeam(UObject* WorldContextObject, uint8 InTeamIndex);

protected:
	virtual void BeginPlay() override;

private:
	ARTSVisionVolume* VisionVolume;

	/** Which tiles are currently unknown, known and visible. */
	TArray<ERTSVisionState> Tiles;

	bool GetTileCoordinates(int Index, int* OutX, int* OutY) const;
	int32 GetTileIndex(int X, int Y) const;

	void NotifyPlayerVisionInfoAvailable();

	UFUNCTION()
	virtual void ReceivedTeamIndex();
};
