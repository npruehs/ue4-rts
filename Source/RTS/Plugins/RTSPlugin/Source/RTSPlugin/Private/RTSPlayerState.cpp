#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerState.h"


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