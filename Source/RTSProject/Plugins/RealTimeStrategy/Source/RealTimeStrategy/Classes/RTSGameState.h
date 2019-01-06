#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/GameStateBase.h"

#include "RTSGameState.generated.h"


class ARTSTeamInfo;


/**
* Game state with RTS features, such as teams.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/** Teams of the current match. */
	UPROPERTY(BlueprintReadOnly, Category = "Team")
	TArray<ARTSTeamInfo*> Teams;
};
