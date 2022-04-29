#include "Orders/RTSOrderData.h"

#include "RTSOrderType.h"
#include "Orders/RTSAttackOrder.h"
#include "Orders/RTSContinueConstructionOrder.h"
#include "Orders/RTSGatherOrder.h"
#include "Orders/RTSMoveOrder.h"
#include "Orders/RTSReturnResourcesOrder.h"

FRTSOrderData::FRTSOrderData()
	: OrderClass(nullptr),
	  TargetActor(nullptr),
	  TargetLocation(FVector::ZeroVector),
	  Index(INDEX_NONE)
{
}

ERTSOrderType FRTSOrderData::GetOrderType() const
{
	if (OrderClass == URTSAttackOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Attack;
	}
	if (OrderClass == URTSContinueConstructionOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_ContinueConstruction;
	}
	if (OrderClass == URTSGatherOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Gather;
	}
	if (OrderClass == URTSMoveOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Move;
	}
	if (OrderClass == URTSReturnResourcesOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_ReturnResources;
	}

	return ERTSOrderType::ORDER_None;
}
