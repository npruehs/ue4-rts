#include "RealTimeStrategyPCH.h"
#include "Vision/RTSVisionComponent.h"


URTSVisionComponent::URTSVisionComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SightRadius = 1000.0f;
}

float URTSVisionComponent::GetSightRadius() const
{
    return SightRadius;
}
