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

	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	AActor* HoveredActor = PlayerController->GetHoveredActor();
	if (HoveredActor != nullptr)
	{
		NotifyDrawHoveredActorEffect(HoveredActor);
	}
}

void ARTSHUD::DrawHealthBar(ARTSCharacter* Character, float CurrentHealth, float MaximumHealth)
{
	FVector2D Center = GetCharacterCenterOnScreen(Character);
	FVector2D Size = GetCharacterSizeOnScreen(Character);

	// Get health bar size.
	const float HealthBarHeight = 15.0f;
	const float HealthBarWidth = Size.X * 2;

	const float HealthBarBorderSize = 3.0f;

	const float HealthPercentage = CurrentHealth / MaximumHealth;

	// Draw health bar border.
	DrawRect(
		FColor::White,
		Center.X + (Size.X / 2) - (HealthBarWidth / 2),
		Center.Y - (Size.Y / 2) - (HealthBarHeight * 2),
		HealthBarWidth,
		HealthBarHeight);

	// Draw health bar.
	DrawRect(
		FColor::Green,
		Center.X + (Size.X / 2) - (HealthBarWidth / 2) + HealthBarBorderSize,
		Center.Y - (Size.Y / 2) - (HealthBarHeight * 2) + HealthBarBorderSize,
		(HealthBarWidth - (HealthBarBorderSize * 2)) * HealthPercentage,
		HealthBarHeight - (HealthBarBorderSize * 2));
}

void ARTSHUD::NotifyDrawHoveredActorEffect(AActor* HoveredActor)
{
	ReceiveDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height)
{
	ReceiveDrawSelectionFrame(ScreenX, ScreenY, Width, Height);
}

FVector2D ARTSHUD::GetCharacterCenterOnScreen(ACharacter* Character)
{
	FVector ProjectedLocation = Project(Character->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetCharacterSizeOnScreen(ACharacter* Character)
{
	// Get character position projected on HUD.
	FCollisionShape CollisionShape = Character->GetCapsuleComponent()->GetCollisionShape();

	FVector CharacterTopPosition = Project(Character->GetActorLocation() + (Character->GetActorUpVector() * CollisionShape.Capsule.HalfHeight));
	FVector CharacterBottomPosition = Project(Character->GetActorLocation() - (Character->GetActorUpVector() * CollisionShape.Capsule.HalfHeight));
	FVector CharacterLeftPosition = Project(Character->GetActorLocation() - (Character->GetActorRightVector() * CollisionShape.Capsule.Radius));
	FVector CharacterRightPosition = Project(Character->GetActorLocation() + (Character->GetActorRightVector() * CollisionShape.Capsule.Radius));

	float Width = FVector(CharacterRightPosition - CharacterLeftPosition).Size();
	float Height = FVector(CharacterTopPosition - CharacterBottomPosition).Size();

	return FVector2D(Width, Height);
}

void ARTSHUD::DrawSelectionFrame()
{
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
	NotifyDrawSelectionFrame(
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
