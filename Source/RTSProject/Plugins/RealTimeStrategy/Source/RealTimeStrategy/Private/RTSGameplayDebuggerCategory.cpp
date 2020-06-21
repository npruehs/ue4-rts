#include "RTSGameplayDebuggerCategory.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "RTSGameplayTagsComponent.h"
#include "RTSPlayerController.h"


FRTSGameplayDebuggerCategory::FRTSGameplayDebuggerCategory()
{
    bShowOnlyWithDebugActor = false;
}

void FRTSGameplayDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
    ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(OwnerPC);

    if (!IsValid(PlayerController))
    {
        return;
    }

    TArray<AActor*> SelectedActors = PlayerController->GetSelectedActors();

    if (SelectedActors.Num() <= 0)
    {
        return;
    }

    AActor* SelectedActor = SelectedActors[0];

    if (!IsValid(SelectedActor))
    {
        return;
    }

    CanvasContext.Printf(TEXT("Selected Actor: %s"), *SelectedActor->GetName());

    URTSGameplayTagsComponent* GameplayTagsComponent = SelectedActor->FindComponentByClass<URTSGameplayTagsComponent>();

    if (IsValid(GameplayTagsComponent))
    {
        CanvasContext.Printf(TEXT(""));
        CanvasContext.Printf(TEXT("Gameplay Tags:"));
        CanvasContext.Printf(TEXT(""));

        for (const FGameplayTag& Tag : GameplayTagsComponent->GetCurrentTags())
        {
            CanvasContext.Printf(TEXT("%s"), *Tag.ToString());
        }
    }
}

TSharedRef<FGameplayDebuggerCategory> FRTSGameplayDebuggerCategory::MakeInstance()
{
    return MakeShareable(new FRTSGameplayDebuggerCategory());
}

#endif // WITH_GAMEPLAY_DEBUGGER
