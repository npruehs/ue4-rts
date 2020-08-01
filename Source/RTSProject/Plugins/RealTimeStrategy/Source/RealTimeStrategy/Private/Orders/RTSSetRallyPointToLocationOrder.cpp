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

void URTSSetRallyPointToLocationOrder::IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    URTSProductionComponent* ProductionComponent = OrderedActor->FindComponentByClass<URTSProductionComponent>();

    if (IsValid(ProductionComponent))
    {
        ProductionComponent->SetRallyPointToLocation(TargetData.Location);
    }
}
