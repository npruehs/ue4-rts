#include "RTSPlayerStart.h"

int32 ARTSPlayerStart::GetTeamIndex() const
{
    return TeamIndex;
}

AController* ARTSPlayerStart::GetPlayer() const
{
    return Player;
}

void ARTSPlayerStart::SetPlayer(AController* InPlayer)
{
    Player = InPlayer;
}
