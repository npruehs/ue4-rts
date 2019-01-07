#include "RealTimeStrategyPCH.h"
#include "RTSGameState.h"

#include "RTSTeamInfo.h"


TArray<ARTSTeamInfo*> ARTSGameState::GetTeams() const
{
    return Teams;
}

void ARTSGameState::AddTeam(ARTSTeamInfo* Team)
{
    if (Teams.Num() <= Team->GetTeamIndex())
    {
        Teams.SetNum(Team->GetTeamIndex() + 1);
    }

    Teams[Team->GetTeamIndex()] = Team;
}
