#include "RTSPluginPrivatePCH.h"
#include "RTSHUD.h"

#include "RTSCharacter.h"
#include "RTSHealthComponent.h"
#include "RTSPlayerController.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelectionFrame();
	DrawHealthBars();
}

void ARTSHUD::DrawHealthBar(ARTSCharacter* Character, float CurrentHealth, float MaximumHealth)
{
	// Get character position projected on HUD.
	FCollisionShape CollisionShape = Character->GetCapsuleComponent()->GetCollisionShape();

	FVector CharacterTopPosition = Project(Character->GetActorLocation() + (Character->GetActorUpVector() * CollisionShape.Capsule.HalfHeight));
	FVector CharacterLeftPosition = Project(Character->GetActorLocation() - (Character->GetActorRightVector() * CollisionShape.Capsule.Radius));
	FVector CharacterRightPosition = Project(Character->GetActorLocation() + (Character->GetActorRightVector() * CollisionShape.Capsule.Radius));

	// Get health bar size.
	const float HealthBarHeight = 15.0f;
	const float HealthBarWidth = FVector(CharacterRightPosition - CharacterLeftPosition).Size() * 2;

	const float HealthBarBorderSize = 3.0f;

	const float HealthPercentage = CurrentHealth / MaximumHealth;

	// Draw health bar border.
	DrawRect(
		FColor::White,
		CharacterTopPosition.X - (HealthBarWidth / 2),
		CharacterTopPosition.Y - (HealthBarHeight * 2),
		HealthBarWidth,
		HealthBarHeight);

	// Draw health bar.
	DrawRect(
		FColor::Green,
		CharacterTopPosition.X - (HealthBarWidth / 2) + HealthBarBorderSize,
		CharacterTopPosition.Y - (HealthBarHeight * 2) + HealthBarBorderSize,
		(HealthBarWidth - (HealthBarBorderSize * 2)) * HealthPercentage,
		HealthBarHeight - (HealthBarBorderSize * 2));
}

void ARTSHUD::DrawSelectionFrame()
{
	if (!SelectionFrameMaterial)
	{
		return;
	}

	// Get selection frame.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	FIntRect SelectionFrame;

	if (!PlayerController->GetSelectionFrame(SelectionFrame))
	{
		return;
	}

	// Draw texture.
	DrawMaterialSimple(
		SelectionFrameMaterial,
		SelectionFrame.Min.X,
		SelectionFrame.Min.Y,
		SelectionFrame.Width(),
		SelectionFrame.Height());
}

void ARTSHUD::DrawHealthBars()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	for (int i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
	{
		AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
		ARTSCharacter* Character = Cast<ARTSCharacter>(SelectedActor);

		if (!IsValid(Character))
		{
			continue;
		}

		// Check if got health.
		URTSHealthComponent* HealthComponent = Character->FindComponentByClass<URTSHealthComponent>();

		if (!HealthComponent)
		{
			continue;
		}

		// Draw health bar.
		DrawHealthBar(Character, HealthComponent->CurrentHealth, HealthComponent->MaximumHealth);
	}
}
