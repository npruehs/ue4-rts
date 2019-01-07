#include "RealTimeStrategyPCH.h"
#include "RTSProductionCostComponent.h"

#include "RTSResourceType.h"


URTSProductionCostComponent::URTSProductionCostComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set reasonable default values.
	ProductionCostType = ERTSProductionCostType::COST_PayImmediately;
	ProductionTime = 5.0f;
	RefundFactor = 0.5f;
}

ERTSProductionCostType URTSProductionCostComponent::GetProductionCostType() const
{
    return ProductionCostType;
}

float URTSProductionCostComponent::GetProductionTime() const
{
    return ProductionTime;
}

TMap<TSubclassOf<URTSResourceType>, float> URTSProductionCostComponent::GetResources() const
{
    return Resources;
}

float URTSProductionCostComponent::GetRefundFactor() const
{
    return RefundFactor;
}
