#include "Orders/RTSGatherOrder.h"

#include "GameFramework/Actor.h"

#include "Economy/RTSGathererComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTargetType.h"


URTSGatherOrder::URTSGatherOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;

    IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanGather());

    IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Constructing());

    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanBeGathered());
    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());

    IssueTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_UnderConstruction());
}

bool URTSGatherOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    if (!IsValid(OrderedActor))
    {
        return false;
    }

    URTSGathererComponent* GathererComponent = OrderedActor->FindComponentByClass<URTSGathererComponent>();
    return IsValid(GathererComponent) && GathererComponent->CanGatherFrom(TargetData.Actor);
}
