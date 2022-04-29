#include "RTSGameplayDebuggerCategory.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "RTSGameplayTagsComponent.h"
#include "RTSPawnAIController.h"
#include "RTSPlayerController.h"


FRTSGameplayDebuggerCategory::FRTSGameplayDebuggerCategory()
{
	bShowOnlyWithDebugActor = false;
}

void FRTSGameplayDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	const ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(OwnerPC);

	if (!IsValid(PlayerController))
	{
		return;
	}

	TArray<AActor*> SelectedActors = PlayerController->GetSelectedActors();

	if (SelectedActors.Num() <= 0)
	{
		return;
	}

	const AActor* SelectedActor = SelectedActors[0];

	if (!IsValid(SelectedActor))
	{
		return;
	}

	// Show selected actor name.
	CanvasContext.Printf(TEXT("Selected Actor: %s"), *SelectedActor->GetName());

	// Show active gameplay tags.
	const URTSGameplayTagsComponent* GameplayTagsComponent = SelectedActor->FindComponentByClass<URTSGameplayTagsComponent>();

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

	// Show current order (server only).
	const APawn* SelectedPawn = Cast<APawn>(SelectedActor);

	if (!IsValid(SelectedPawn))
	{
		return;
	}

	const ARTSPawnAIController* PawnAIController = SelectedPawn->GetController<ARTSPawnAIController>();

	if (IsValid(PawnAIController))
	{
		const TSubclassOf<URTSOrder> CurrentOrder = PawnAIController->GetCurrentOrder();
		const FString OrderName = CurrentOrder != nullptr ? CurrentOrder->GetName() : TEXT("none");

		CanvasContext.Printf(TEXT(""));
		CanvasContext.Printf(TEXT("Current Order: %s"), *OrderName);
	}
}

TSharedRef<FGameplayDebuggerCategory> FRTSGameplayDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FRTSGameplayDebuggerCategory());
}

#endif // WITH_GAMEPLAY_DEBUGGER
