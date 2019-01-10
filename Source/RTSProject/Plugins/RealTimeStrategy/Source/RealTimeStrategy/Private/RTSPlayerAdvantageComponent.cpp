#include "RTSPlayerAdvantageComponent.h"

URTSPlayerAdvantageComponent::URTSPlayerAdvantageComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    bGodModeEnabled = false;
    SpeedBoostFactor = 1.0f;
}

bool URTSPlayerAdvantageComponent::IsGodModeEnabled() const
{
    return bGodModeEnabled;
}

float URTSPlayerAdvantageComponent::GetSpeedBoostFactor() const
{
    return SpeedBoostFactor;
}

void URTSPlayerAdvantageComponent::SetGodModeEnabled(bool bInGodModeEnabled)
{
    bGodModeEnabled = bInGodModeEnabled;
}

void URTSPlayerAdvantageComponent::SetSpeedBoostFactor(float InSpeedBoostFactor)
{
    SpeedBoostFactor = InSpeedBoostFactor;
}
