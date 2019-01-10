#pragma once

#include "CoreMinimal.h"

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
    /** Gets the teams of the current match. */
    UFUNCTION(BlueprintPure)
    TArray<ARTSTeamInfo*> GetTeams() const;

    /** Sets the team with the specified index. */
    void AddTeam(ARTSTeamInfo* Team);

private:
	/** Teams of the current match. */
	UPROPERTY()
	TArray<ARTSTeamInfo*> Teams;
};
