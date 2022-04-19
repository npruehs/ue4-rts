#include "RTSPlayerAdvantageComponent.h"

URTSPlayerAdvantageComponent::URTSPlayerAdvantageComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	bGodModeEnabled = false;
	OutgoingDamageFactor = 1.0f;
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

float URTSPlayerAdvantageComponent::GetOutgoingDamageFactor() const
{
	return OutgoingDamageFactor;
}

void URTSPlayerAdvantageComponent::SetGodModeEnabled(bool bInGodModeEnabled)
{
	bGodModeEnabled = bInGodModeEnabled;
}

void URTSPlayerAdvantageComponent::SetSpeedBoostFactor(float InSpeedBoostFactor)
{
	SpeedBoostFactor = InSpeedBoostFactor;
}

void URTSPlayerAdvantageComponent::SetOutgoingDamageFactor(float InOutgoingDamageFactor)
{
	OutgoingDamageFactor = InOutgoingDamageFactor;
}
