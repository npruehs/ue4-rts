#include "RTSPluginPrivatePCH.h"
#include "RTSTeamInfo.h"

#include "RTSGameState.h"
#include "RTSPlayerState.h"


ARTSTeamInfo::ARTSTeamInfo()
{
	SetReplicates(true);
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;

	// Force ReceivedTeamIndex() on clients.
	TeamIndex = 255;
}

void ARTSTeamInfo::AddToTeam(AController* Player)
{
	if (Player == nullptr)
	{
		return;
	}

	ARTSPlayerState* PlayerState = Cast<ARTSPlayerState>(Player->PlayerState);
	if (PlayerState == nullptr)
	{
		return;
	}

	if (PlayerState->Team != nullptr)
	{
		RemoveFromTeam(Player);
	}

	PlayerState->Team = this;
	PlayerState->NotifyOnTeamChanged(this);

	TeamMembers.Add(Player);
}

void ARTSTeamInfo::RemoveFromTeam(AController* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	
	if (!TeamMembers.Contains(Player))
	{
		return;
	}

	TeamMembers.Remove(Player);

	ARTSPlayerState* PlayerState = Cast<ARTSPlayerState>(Player->PlayerState);

	if (PlayerState != nullptr)
	{
		PlayerState->Team = nullptr;
		PlayerState->NotifyOnTeamChanged(nullptr);
	}
}

TArray<AController*> ARTSTeamInfo::GetTeamMembers()
{
	return TeamMembers;
}

void ARTSTeamInfo::ReceivedTeamIndex()
{
	if (TeamIndex == 255)
	{
		return;
	}

	ARTSGameState* GameState = GetWorld()->GetGameState<ARTSGameState>();
	if (GameState == nullptr)
	{
		return;
	}

	if (GameState->Teams.Num() <= TeamIndex)
	{
		GameState->Teams.SetNum(TeamIndex + 1);
	}

	GameState->Teams[TeamIndex] = this;
}

void ARTSTeamInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARTSTeamInfo, TeamIndex, COND_InitialOnly);
}
