#include "Orders/RTSSetRallyPointToActorOrder.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Production/RTSProductionComponent.h"

URTSSetRallyPointToActorOrder::URTSSetRallyPointToActorOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;
    GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

    IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanProduce());

    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
}

void URTSSetRallyPointToActorOrder::IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    URTSProductionComponent* ProductionComponent = OrderedActor->FindComponentByClass<URTSProductionComponent>();

    if (IsValid(ProductionComponent))
    {
        ProductionComponent->SetRallyPointToActor(TargetData.Actor);
    }
}
