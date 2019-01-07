#include "RealTimeStrategyPCH.h"
#include "RTSVisionComponent.h"


URTSVisionComponent::URTSVisionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SightRadius = 1000.0f;
}

float URTSVisionComponent::GetSightRadius() const
{
    return SightRadius;
}
