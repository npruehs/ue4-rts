#pragma once

#include <CoreMinimal.h>
#include <LatentActions.h>
#include <Engine/LatentActionManager.h>

class ADaeTestTriggerBox;

/** Triggers the output link after the specified trigger box has been triggered. */
class DAEDALICTESTAUTOMATIONPLUGIN_API FDaeDelayUntilTriggeredAction : public FPendingLatentAction
{
public:
    ADaeTestTriggerBox* TestTriggerBox;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FDaeDelayUntilTriggeredAction(const FLatentActionInfo& LatentInfo,
                                  ADaeTestTriggerBox* InTestTriggerBox);

    virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
    virtual FString GetDescription() const override;
#endif
};
