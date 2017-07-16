#include "RTSPluginPrivatePCH.h"
#include "RTSHealthComponent.h"

#include "Net/UnrealNetwork.h"


void URTSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSHealthComponent, CurrentHealth);
}
