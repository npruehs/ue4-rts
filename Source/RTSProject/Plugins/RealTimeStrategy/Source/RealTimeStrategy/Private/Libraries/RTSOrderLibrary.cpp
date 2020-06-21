#include "Libraries/RTSOrderLibrary.h"

#include "GameFramework/Actor.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTagRequirements.h"


bool URTSOrderLibrary::IsValidOrderTarget(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index)
{
    if (OrderClass == nullptr)
    {
        return false;
    }

    const URTSOrder* Order = OrderClass->GetDefaultObject<URTSOrder>();

    ERTSOrderTargetType TargetType = Order->GetTargetType();

    if (TargetType == ERTSOrderTargetType::ORDERTARGET_Actor)
    {
        if (!IsValid(TargetData.Actor))
        {
            return false;
        }

        FRTSOrderTagRequirements TagRequirements = Order->GetIssueTagRequirements();

        if (!URTSGameplayTagLibrary::MeetsTagRequirements(TargetData.TargetTags, TagRequirements.TargetRequiredTags, TagRequirements.TargetBlockedTags))
        {
            return false;
        }
    }

    return Order->IsValidTarget(OrderedActor, TargetData, Index);
}

ERTSOrderGroupExecutionType URTSOrderLibrary::GetOrderGroupExecutionType(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetGroupExecutionType() : ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;
}

FRTSOrderTargetData URTSOrderLibrary::GetOrderTargetData(const AActor* OrderedActor, AActor* TargetActor, const FVector& TargetLocation /*= FVector::ZeroVector*/)
{
    FRTSOrderTargetData TargetData;
    TargetData.Actor = TargetActor;
    TargetData.Location = TargetLocation;

    if (!IsValid(TargetActor))
    {
        return TargetData;
    }

    FGameplayTagContainer SourceTags;
    FGameplayTagContainer TargetTags;
    
    GetSourceAndTargetTags(OrderedActor, TargetActor, SourceTags, TargetTags);

    TargetData.TargetTags = TargetTags;
    return TargetData;
}

void URTSOrderLibrary::GetSourceAndTargetTags(const AActor* SourceActor, const AActor* TargetActor, FGameplayTagContainer& OutSourceTags, FGameplayTagContainer& OutTargetTags)
{
    OutSourceTags = URTSGameplayTagLibrary::GetGameplayTags(SourceActor);
    OutTargetTags = URTSGameplayTagLibrary::GetGameplayTags(TargetActor);

    FGameplayTagContainer RelationshipTags = URTSGameplayTagLibrary::GetActorRelationshipTags(SourceActor, TargetActor);

    OutSourceTags.AppendTags(RelationshipTags);
    OutTargetTags.AppendTags(RelationshipTags);
}
