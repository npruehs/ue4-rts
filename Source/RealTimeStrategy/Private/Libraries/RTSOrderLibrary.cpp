#include "Libraries/RTSOrderLibrary.h"

#include "GameFramework/Actor.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderTagRequirements.h"


bool URTSOrderLibrary::CanObeyOrder(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, int32 Index)
{
    if (OrderClass == nullptr)
    {
        return false;
    }

    if (!IsValid(OrderedActor))
    {
        return false;
    }

    const URTSOrder* Order = OrderClass->GetDefaultObject<URTSOrder>();

    FGameplayTagContainer OrderedActorTags = URTSGameplayTagLibrary::GetGameplayTags(OrderedActor);
    FRTSOrderTagRequirements TagRequirements = Order->GetIssueTagRequirements();

    if (!URTSGameplayTagLibrary::MeetsTagRequirements(OrderedActorTags, TagRequirements.SourceRequiredTags, TagRequirements.SourceBlockedTags))
    {
        return false;
    }

    return Order->CanObeyOrder(OrderedActor, Index);
}

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

void URTSOrderLibrary::IssueOrder(TSubclassOf<URTSOrder> OrderClass, AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index)
{
    if (OrderClass == nullptr)
    {
        return;
    }

    OrderClass->GetDefaultObject<URTSOrder>()->IssueOrder(OrderedActor, TargetData, Index);
}

void URTSOrderLibrary::IssueOrder(AActor* OrderedActor, const FRTSOrderData& Order)
{
    if (!IsValid(OrderedActor))
    {
        return;
    }

    FRTSOrderTargetData OrderTargetData = GetOrderTargetData(OrderedActor, Order.TargetActor, Order.TargetLocation);
    IssueOrder(Order.OrderClass, OrderedActor, OrderTargetData, Order.Index);
}

ERTSOrderGroupExecutionType URTSOrderLibrary::GetOrderGroupExecutionType(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetGroupExecutionType() : ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;
}

UTexture2D* URTSOrderLibrary::GetOrderNormalIcon(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetNormalIcon() : nullptr;
}

UTexture2D* URTSOrderLibrary::GetOrderHoveredIcon(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetHoveredIcon() : nullptr;
}

UTexture2D* URTSOrderLibrary::GetOrderPressedIcon(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetPressedIcon() : nullptr;
}

UTexture2D* URTSOrderLibrary::GetOrderDisabledIcon(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetDisabledIcon() : nullptr;
}

FText URTSOrderLibrary::GetOrderName(TSubclassOf<URTSOrder> OrderClass)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetName() : FText();
}

FText URTSOrderLibrary::GetOrderDescription(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, int32 Index)
{
    return OrderClass != nullptr ? OrderClass->GetDefaultObject<URTSOrder>()->GetDescription(OrderedActor, Index) : FText();
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
