#include "RTSPluginPrivatePCH.h"
#include "RTSHUD.h"

#include "EngineUtils.h"

#include "RTSCharacter.h"
#include "RTSHealthComponent.h"
#include "RTSPlayerController.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelectionFrame();
	DrawHealthBars();
	DrawHoveredActorEffect();
}

void ARTSHUD::NotifyDrawHoveredActorEffect(AActor* HoveredActor)
{
	ReceiveDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::NotifyDrawHealthBar(
	ARTSCharacter* Character,
	float CurrentHealth,
	float MaximumHealth,
	float HealthPercentage,
	float SuggestedHealthBarLeft,
	float SuggestedHealthBarTop,
	float SuggestedHealthBarWidth,
	float SuggestedHealthBarHeight)
{
	ReceiveDrawHealthBar(
		Character,
		CurrentHealth,
		MaximumHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
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

	// Draw selection frame.
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

	// Check override conditions.
	if (bAlwaysShowHealthBars || (bShowHotkeyHealthBars && PlayerController->IsHealthBarHotkeyPressed()))
	{
		// Draw all health bars.
		for (TActorIterator<ARTSCharacter> CharacterIt(GetWorld()); CharacterIt; ++CharacterIt)
		{
			ARTSCharacter* Character = *CharacterIt;
			DrawHealthBar(Character);
		}

		return;
	}

	// Draw health bar for hovered character.
	if (bShowHoverHealthBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		ARTSCharacter* Character = Cast<ARTSCharacter>(HoveredActor);
		DrawHealthBar(Character);
	}

	// Draw health bars for selected characters.
	if (bShowSelectionHealthBars)
	{
		for (int i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			ARTSCharacter* Character = Cast<ARTSCharacter>(SelectedActor);
			DrawHealthBar(Character);
		}
	}
}

void ARTSHUD::DrawHealthBar(ARTSCharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	// Check health.
	URTSHealthComponent* HealthComponent = Character->FindComponentByClass<URTSHealthComponent>();

	if (!HealthComponent)
	{
		return;
	}

	const float HealthPercentage = HealthComponent->CurrentHealth / HealthComponent->MaximumHealth;

	// Suggest health bar size.
	FVector2D Center = GetCharacterCenterOnScreen(Character);
	FVector2D Size = GetCharacterSizeOnScreen(Character);

	const float SuggestedHealthBarWidth = Size.X * 2;
	const float SuggestedHealthBarHeight = 15.0f;

	const float SuggestedHealthBarLeft = Center.X - (SuggestedHealthBarWidth / 2);
	const float SuggestedHealthBarTop = Center.Y - (Size.Y / 2) - (SuggestedHealthBarHeight * 2);

	// Draw health bar.
	NotifyDrawHealthBar(
		Character,
		HealthComponent->CurrentHealth,
		HealthComponent->MaximumHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::DrawHoveredActorEffect()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	AActor* HoveredActor = PlayerController->GetHoveredActor();

	if (HoveredActor == nullptr)
	{
		return;
	}

	// Draw hovered actor effect.
	NotifyDrawHoveredActorEffect(HoveredActor);
}
