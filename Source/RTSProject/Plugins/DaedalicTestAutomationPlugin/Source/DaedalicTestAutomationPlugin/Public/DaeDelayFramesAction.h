#pragma once

#include <CoreMinimal.h>
#include <LatentActions.h>
#include <Engine/LatentActionManager.h>

/** Triggers the output link after the specified number of frames. */
class DAEDALICTESTAUTOMATIONPLUGIN_API FDaeDelayFramesAction : public FPendingLatentAction
{
public:
    int32 FramesRemaining;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FDaeDelayFramesAction(const FLatentActionInfo& LatentInfo, int32 NumFrames);

    virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
    virtual FString GetDescription() const override;
#endif
};
