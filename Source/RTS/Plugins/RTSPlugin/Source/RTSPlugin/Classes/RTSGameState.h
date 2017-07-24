#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/GameState.h"

#include "RTSGameState.generated.h"


class ARTSTeamInfo;


UCLASS()
class ARTSGameState : public AGameState
{
	GENERATED_BODY()

public:
	/** Teams of the current match. */
	UPROPERTY(BlueprintReadOnly, Category = Team)
	TArray<ARTSTeamInfo*> Teams;
};