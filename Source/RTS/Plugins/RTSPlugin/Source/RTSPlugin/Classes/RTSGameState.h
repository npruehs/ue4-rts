#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/GameState.h"

#include "RTSGameState.generated.h"


class ARTSTeamInfo;


/**
* Game state with RTS features, such as teams.
*/
UCLASS()
class ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/** Teams of the current match. */
	UPROPERTY(BlueprintReadOnly, Category = Team)
	TArray<ARTSTeamInfo*> Teams;
};