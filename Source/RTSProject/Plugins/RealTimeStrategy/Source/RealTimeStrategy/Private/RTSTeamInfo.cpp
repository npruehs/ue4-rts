#include "RTSTeamInfo.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

#include "RTSGameState.h"
#include "RTSPlayerState.h"


ARTSTeamInfo::ARTSTeamInfo(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	SetReplicates(true);
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;

	// Force ReceivedTeamIndex() on clients.
	TeamIndex = 255;
}

void ARTSTeamInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ARTSTeamInfo, TeamIndex, COND_InitialOnly);
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

	if (PlayerState->GetTeam() != nullptr)
	{
		RemoveFromTeam(Player);
	}

	PlayerState->SetTeam(this);
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
		PlayerState->SetTeam(nullptr);
		PlayerState->NotifyOnTeamChanged(nullptr);
	}
}

uint8 ARTSTeamInfo::GetTeamIndex() const
{
    return TeamIndex;
}

TArray<AController*> ARTSTeamInfo::GetTeamMembers() const
{
	return TeamMembers;
}

void ARTSTeamInfo::SetTeamIndex(uint8 InTeamIndex)
{
    TeamIndex = InTeamIndex;
}
