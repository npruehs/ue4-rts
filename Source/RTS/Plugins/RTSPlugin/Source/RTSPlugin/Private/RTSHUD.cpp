#include "RTSPluginPrivatePCH.h"
#include "RTSHUD.h"

#include "EngineUtils.h"

#include "RTSCharacter.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSHealthComponent.h"
#include "RTSPlayerController.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelectionFrame();
	DrawHealthBars();
	DrawConstructionProgressBars();
	DrawHoveredActorEffect();
}

void ARTSHUD::NotifyDrawConstructionProgressBar(ARTSCharacter* Character, float ConstructionTime, float RemainingConstructionTime, float ProgressPercentage, float SuggestedProgressBarLeft, float SuggestedProgressBarTop, float SuggestedProgressBarWidth, float SuggestedProgressBarHeight)
{
	ReceiveDrawConstructionProgressBar(
		Character,
		ConstructionTime,
		RemainingConstructionTime,
		ProgressPercentage,
		SuggestedProgressBarLeft,
		SuggestedProgressBarTop,
		SuggestedProgressBarWidth,
		SuggestedProgressBarHeight);
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

FVector2D ARTSHUD::GetCharacterCenterOnScreen(ACharacter* Character) const
{
	FVector ProjectedLocation = Project(Character->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetCharacterSizeOnScreen(ACharacter* Character) const
{
	// Get character position projected on HUD.
	FCollisionShape CollisionShape = Character->GetCapsuleComponent()->GetCollisionShape();

	FVector CharacterTopPosition = Project(Character->GetActorLocation() + (Character->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector CharacterBottomPosition = Project(Character->GetActorLocation() - (Character->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
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
	float SuggestedHealthBarLeft;
	float SuggestedHealthBarTop;
	float SuggestedHealthBarWidth;
	float SuggestedHealthBarHeight;

	SuggestUnitBarSize(Character, SuggestedHealthBarLeft, SuggestedHealthBarTop, SuggestedHealthBarWidth, SuggestedHealthBarHeight);

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

void ARTSHUD::DrawConstructionProgressBars()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	// Check override conditions.
	if (bAlwaysShowConstructionProgressBars || (bShowHotkeyConstructionProgressBars && PlayerController->IsConstructionProgressBarHotkeyPressed()))
	{
		// Draw all progress bars.
		for (TActorIterator<ARTSCharacter> CharacterIt(GetWorld()); CharacterIt; ++CharacterIt)
		{
			ARTSCharacter* Character = *CharacterIt;
			DrawConstructionProgressBar(Character);
		}

		return;
	}

	// Draw progress bar for hovered character.
	if (bShowHoverConstructionProgressBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		ARTSCharacter* Character = Cast<ARTSCharacter>(HoveredActor);
		DrawConstructionProgressBar(Character);
	}

	// Draw progress bars for selected characters.
	if (bShowSelectionConstructionProgressBars)
	{
		for (int i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			ARTSCharacter* Character = Cast<ARTSCharacter>(SelectedActor);
			DrawConstructionProgressBar(Character);
		}
	}
}

void ARTSHUD::DrawConstructionProgressBar(ARTSCharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	// Check progress.
	URTSConstructionSiteComponent* ConstructionSiteComponent = Character->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent)
	{
		return;
	}

	if (!ConstructionSiteComponent->IsConstructing())
	{
		return;
	}

	const float ProgressPercentage = ConstructionSiteComponent->GetProgressPercentage();

	// Suggest progress bar size.
	float SuggestedProgressBarLeft;
	float SuggestedProgressBarTop;
	float SuggestedProgressBarWidth;
	float SuggestedProgressBarHeight;
	
	SuggestUnitBarSize(Character, SuggestedProgressBarLeft, SuggestedProgressBarTop, SuggestedProgressBarWidth, SuggestedProgressBarHeight);

	// Draw progress bar.
	NotifyDrawConstructionProgressBar(
		Character,
		ConstructionSiteComponent->ConstructionTime,
		ConstructionSiteComponent->RemainingConstructionTime,
		ProgressPercentage,
		SuggestedProgressBarLeft,
		SuggestedProgressBarTop,
		SuggestedProgressBarWidth,
		SuggestedProgressBarHeight);
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

void ARTSHUD::SuggestUnitBarSize(ARTSCharacter* Character, float& OutLeft, float& OutTop, float& OutWidth, float& OutHeight) const
{
	FVector2D Center = GetCharacterCenterOnScreen(Character);
	FVector2D Size = GetCharacterSizeOnScreen(Character);

	OutWidth = Size.X * 2;
	OutHeight = 15.0f;

	OutLeft = Center.X - (OutWidth / 2);
	OutTop = Center.Y - (Size.Y / 2) - (OutHeight * 2);
}
