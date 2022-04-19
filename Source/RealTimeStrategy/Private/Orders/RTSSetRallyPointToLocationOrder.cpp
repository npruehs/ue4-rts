#include "Orders/RTSSetRallyPointToLocationOrder.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Production/RTSProductionComponent.h"

URTSSetRallyPointToLocationOrder::URTSSetRallyPointToLocationOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TargetType = ERTSOrderTargetType::ORDERTARGET_Location;
	GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

	IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanProduce());
}

void URTSSetRallyPointToLocationOrder::AddOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	SetOrder(OrderedActor, TargetData, Index);
}

void URTSSetRallyPointToLocationOrder::SetOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	if (URTSProductionComponent* ProductionComponent = OrderedActor->FindComponentByClass<URTSProductionComponent>(); IsValid(ProductionComponent))
	{
		ProductionComponent->SetRallyPointToLocation(TargetData.Location);
	}
}
