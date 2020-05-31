#include "DaeTestBlueprintFunctionLibrary.h"
#include "DaeDelayFramesAction.h"
#include "DaeDelayUntilTriggeredAction.h"
#include <Engine/Engine.h>
#include <Engine/LatentActionManager.h>
#include <Engine/World.h>

void UDaeTestBlueprintFunctionLibrary::DelayFrames(UObject* WorldContextObject,
                                                   struct FLatentActionInfo LatentInfo,
                                                   int32 NumFrames)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
                                                           EGetWorldErrorMode::LogAndReturnNull))
    {
        FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FDaeDelayFramesAction>(LatentInfo.CallbackTarget,
                                                                          LatentInfo.UUID)
            == NULL)
        {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FDaeDelayFramesAction(LatentInfo, NumFrames));
        }
    }
}

void UDaeTestBlueprintFunctionLibrary::DelayUntilTriggered(UObject* WorldContextObject,
                                                           struct FLatentActionInfo LatentInfo,
                                                           ADaeTestTriggerBox* TestTriggerBox)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
                                                           EGetWorldErrorMode::LogAndReturnNull))
    {
        FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FDaeDelayUntilTriggeredAction>(
                LatentInfo.CallbackTarget, LatentInfo.UUID)
            == NULL)
        {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FDaeDelayUntilTriggeredAction(LatentInfo,
                                                                               TestTriggerBox));
        }
    }
}
