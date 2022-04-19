#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"

#include "GameplayDebuggerCategory.h"

class FRTSGameplayDebuggerCategory : public FGameplayDebuggerCategory
{
public:
    FRTSGameplayDebuggerCategory();

    virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

    static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
};

#endif // WITH_GAMEPLAY_DEBUGGER
