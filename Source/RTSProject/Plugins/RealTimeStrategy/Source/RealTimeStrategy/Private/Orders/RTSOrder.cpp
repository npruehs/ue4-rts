#include "Orders/RTSOrder.h"

bool URTSOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    return true;
}

float URTSOrder::GetRequiredRange(const AActor* OrderedActor, int32 Index) const
{
    return 0.0f;
}

ERTSOrderTargetType URTSOrder::GetTargetType() const
{
    return TargetType;
}

FRTSOrderTagRequirements URTSOrder::GetIssueTagRequirements() const
{
    return IssueTagRequirements;
}
