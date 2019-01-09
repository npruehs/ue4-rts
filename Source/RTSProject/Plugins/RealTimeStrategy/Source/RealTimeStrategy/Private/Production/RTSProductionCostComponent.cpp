#include "RealTimeStrategyPCH.h"
#include "Production/RTSProductionCostComponent.h"

#include "Economy/RTSResourceType.h"


URTSProductionCostComponent::URTSProductionCostComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	// Set reasonable default values.
	ProductionCostType = ERTSPaymentType::PAYMENT_PayImmediately;
	ProductionTime = 5.0f;
	RefundFactor = 0.5f;
}

ERTSPaymentType URTSProductionCostComponent::GetProductionCostType() const
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
