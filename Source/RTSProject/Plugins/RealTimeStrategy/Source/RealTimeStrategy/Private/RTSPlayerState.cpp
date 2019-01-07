#include "RealTimeStrategyPCH.h"
#include "RTSPlayerState.h"

#include "RTSPlayerController.h"
#include "RTSTeamInfo.h"


void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSPlayerState, Team);
}

ARTSTeamInfo* ARTSPlayerState::GetTeam() const
{
    return Team;
}

void ARTSPlayerState::SetTeam(ARTSTeamInfo* InTeam)
{
    Team = InTeam;
}

bool ARTSPlayerState::IsSameTeamAs(ARTSPlayerState* Other) const
{
	if (!Other)
	{
		return false;
	}

	ARTSTeamInfo* FirstTeam = Team;
	ARTSTeamInfo* SecondTeam = Other->Team;

	if (!FirstTeam || !SecondTeam)
	{
		return false;
	}

	return FirstTeam->GetTeamIndex() == SecondTeam->GetTeamIndex();
}

void ARTSPlayerState::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team %d."), *GetName(), NewTeam->GetTeamIndex());
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team None."), *GetName());
	}

	// Notify listeners.
	ReceiveOnTeamChanged(NewTeam);

	// Notify player.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetOwner());

	if (PlayerController)
	{
		PlayerController->NotifyOnTeamChanged(NewTeam);
	}
}

void ARTSPlayerState::OnTeamChanged()
{
    NotifyOnTeamChanged(Team);
}
