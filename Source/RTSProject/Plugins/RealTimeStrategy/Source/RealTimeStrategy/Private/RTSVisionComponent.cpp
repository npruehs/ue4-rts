#include "RealTimeStrategyPCH.h"
#include "RTSVisionComponent.h"


URTSVisionComponent::URTSVisionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicated(true);

	// Set reasonable default values.
	SightRadius = 1000.0f;
}

void URTSVisionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSVisionComponent, SightRadius);
}
