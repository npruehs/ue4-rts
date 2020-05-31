#include "DaeDelayUntilTriggeredAction.h"
#include "DaeTestTriggerBox.h"

FDaeDelayUntilTriggeredAction::FDaeDelayUntilTriggeredAction(const FLatentActionInfo& LatentInfo,
                                                             ADaeTestTriggerBox* InTestTriggerBox)
    : TestTriggerBox(InTestTriggerBox)
    , ExecutionFunction(LatentInfo.ExecutionFunction)
    , OutputLink(LatentInfo.Linkage)
    , CallbackTarget(LatentInfo.CallbackTarget)
{
}

void FDaeDelayUntilTriggeredAction::UpdateOperation(FLatentResponse& Response)

{
    bool bWasTriggered = !IsValid(TestTriggerBox) || TestTriggerBox->WasTriggered();
    Response.FinishAndTriggerIf(bWasTriggered, ExecutionFunction, OutputLink, CallbackTarget);
}

#if WITH_EDITOR
FString FDaeDelayUntilTriggeredAction::GetDescription() const
{
    FString TriggerBoxName = IsValid(TestTriggerBox) ? TestTriggerBox->GetName() : TEXT("nullptr");
    return FString::Printf(TEXT("Delay (until {0} was triggered)"), *TriggerBoxName);
}
#endif
