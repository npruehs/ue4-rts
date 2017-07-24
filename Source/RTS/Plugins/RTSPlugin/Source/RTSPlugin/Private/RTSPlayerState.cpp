#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerState.h"


void ARTSPlayerState::NotifyOnTeamChanged()
{
	ReceiveOnTeamChanged();
}

void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSPlayerState, Team);
}