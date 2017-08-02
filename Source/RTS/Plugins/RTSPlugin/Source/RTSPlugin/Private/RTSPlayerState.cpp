#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerState.h"

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

void ARTSPlayerState::NotifyOnTeamChanged()
{
	UE_LOG(RTSLog, Log, TEXT("Player %s added to team %d."), *GetName(), Team->TeamIndex);

	ReceiveOnTeamChanged();
}

void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSPlayerState, Team);
}