#include "RTSPluginPCH.h"
#include "RTSPlayerAdvantageComponent.h"

URTSPlayerAdvantageComponent::URTSPlayerAdvantageComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bGodModeEnabled = false;
    SpeedBoostFactor = 1.0f;
}
