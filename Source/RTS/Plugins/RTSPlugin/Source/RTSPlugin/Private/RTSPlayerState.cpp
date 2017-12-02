#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerState.h"

#include "RTSPlayerController.h"
#include "RTSTeamInfo.h"


bool ARTSPlayerState::IsSameTeamAs(ARTSPlayerState* Other)
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

	return FirstTeam->TeamIndex == SecondTeam->TeamIndex;
}

void ARTSPlayerState::OnTeamChanged()
{
	NotifyOnTeamChanged(Team);
}

void ARTSPlayerState::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		UE_LOG(RTSLog, Log, TEXT("Player %s added to team %d."), *GetName(), NewTeam->TeamIndex);
	}
	else
	{
		UE_LOG(RTSLog, Log, TEXT("Player %s added to team None."), *GetName());
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

void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSPlayerState, Team);
}
