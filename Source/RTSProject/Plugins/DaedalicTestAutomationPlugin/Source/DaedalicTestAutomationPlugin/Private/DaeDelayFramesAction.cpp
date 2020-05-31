#include "DaeDelayFramesAction.h"

FDaeDelayFramesAction::FDaeDelayFramesAction(const FLatentActionInfo& LatentInfo, int32 NumFrames)
    : FramesRemaining(NumFrames)
    , ExecutionFunction(LatentInfo.ExecutionFunction)
    , OutputLink(LatentInfo.Linkage)
    , CallbackTarget(LatentInfo.CallbackTarget)
{
}

void FDaeDelayFramesAction::UpdateOperation(FLatentResponse& Response)

{
    --FramesRemaining;
    Response.FinishAndTriggerIf(FramesRemaining <= 0, ExecutionFunction, OutputLink,
                                CallbackTarget);
}

#if WITH_EDITOR
FString FDaeDelayFramesAction::GetDescription() const
{
    return FString::Printf(TEXT("Delay ({0} frames left)"), FramesRemaining);
}
#endif
