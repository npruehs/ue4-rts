#include "Orders/RTSOrder.h"

bool URTSOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    return true;
}

ERTSOrderTargetType URTSOrder::GetTargetType() const
{
    return TargetType;
}

ERTSOrderGroupExecutionType URTSOrder::GetGroupExecutionType() const
{
    return GroupExecutionType;
}

FRTSOrderTagRequirements URTSOrder::GetIssueTagRequirements() const
{
    return IssueTagRequirements;
}

UTexture2D* URTSOrder::GetNormalIcon() const
{
    return NormalIcon;
}

UTexture2D* URTSOrder::GetHoveredIcon() const
{
    return HoveredIcon;
}

UTexture2D* URTSOrder::GetPressedIcon() const
{
    return PressedIcon;
}

UTexture2D* URTSOrder::GetDisabledIcon() const
{
    return DisabledIcon;
}

FText URTSOrder::GetName() const
{
    return Name;
}

FText URTSOrder::GetDescription(const AActor* OrderedActor, int32 Index) const
{
    return Description;
}
