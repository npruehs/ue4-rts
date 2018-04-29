#include "RTSPluginPCH.h"
#include "RTSProductionCostComponent.h"

#include "RTSResourceType.h"


URTSProductionCostComponent::URTSProductionCostComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set reasonable default values.
	ProductionCostType = ERTSProductionCostType::COST_PayImmediately;
	ProductionTime = 5.0f;
	RefundFactor = 0.5f;
}
