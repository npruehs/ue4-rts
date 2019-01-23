#include "UI/RTSHUD.h"

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"

#include "RTSPlayerController.h"
#include "Combat/RTSHealthComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Production/RTSProductionComponent.h"
#include "UI/RTSFloatingCombatTextComponent.h"
#include "UI/RTSFloatingCombatTextData.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

    DrawFloatingCombatTexts();
	DrawSelectionFrame();
	DrawHealthBars();
	DrawConstructionProgressBars();
	DrawProductionProgressBars();
	DrawHoveredActorEffect();
}

void ARTSHUD::NotifyDrawConstructionProgressBar(AActor* Actor, float ConstructionTime, float RemainingConstructionTime, float ProgressPercentage, float SuggestedProgressBarLeft, float SuggestedProgressBarTop, float SuggestedProgressBarWidth, float SuggestedProgressBarHeight)
{
	ReceiveDrawConstructionProgressBar(
		Actor,
		ConstructionTime,
		RemainingConstructionTime,
		ProgressPercentage,
		SuggestedProgressBarLeft,
		SuggestedProgressBarTop,
		SuggestedProgressBarWidth,
		SuggestedProgressBarHeight);
}

void ARTSHUD::NotifyDrawFloatingCombatText(AActor* Actor, const FString& Text, const FLinearColor& Color, float Scale, float Lifetime, float RemainingLifetime, float LifetimePercentage, float SuggestedTextLeft, float SuggestedTextTop)
{
    ReceiveDrawFloatingCombatText(
        Actor,
        Text,
        Color,
        Scale,
        Lifetime,
        RemainingLifetime,
        LifetimePercentage,
        SuggestedTextLeft,
        SuggestedTextTop);
}

void ARTSHUD::NotifyDrawHoveredActorEffect(AActor* HoveredActor)
{
	ReceiveDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::NotifyDrawHealthBar(
	AActor* Actor,
	float CurrentHealth,
	float MaximumHealth,
	float HealthPercentage,
	float SuggestedHealthBarLeft,
	float SuggestedHealthBarTop,
	float SuggestedHealthBarWidth,
	float SuggestedHealthBarHeight)
{
	ReceiveDrawHealthBar(
		Actor,
		CurrentHealth,
		MaximumHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::NotifyDrawProductionProgressBar(AActor* Actor, float ProductionTime, float RemainingProductionTime, float ProgressPercentage, float SuggestedProgressBarLeft, float SuggestedProgressBarTop, float SuggestedProgressBarWidth, float SuggestedProgressBarHeight)
{
	ReceiveDrawProductionProgressBar(
		Actor,
		ProductionTime,
		RemainingProductionTime,
		ProgressPercentage,
		SuggestedProgressBarLeft,
		SuggestedProgressBarTop,
		SuggestedProgressBarWidth,
		SuggestedProgressBarHeight);
}

void ARTSHUD::NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height)
{
	ReceiveDrawSelectionFrame(ScreenX, ScreenY, Width, Height);
}

FVector2D ARTSHUD::GetActorCenterOnScreen(AActor* Actor) const
{
	FVector ProjectedLocation = Project(Actor->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetActorSizeOnScreen(AActor* Actor) const
{
	// Get actor position projected on HUD.
	UCapsuleComponent* CapsuleComponent = Actor->FindComponentByClass<UCapsuleComponent>();

	if (!CapsuleComponent)
	{
		return FVector2D::ZeroVector;
	}

	FCollisionShape CollisionShape = CapsuleComponent->GetCollisionShape();

	FVector ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));
	FVector ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));

	float Width = FVector(ActorRightPosition - ActorLeftPosition).Size();
	float Height = FVector(ActorTopPosition - ActorBottomPosition).Size();

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

void ARTSHUD::DrawFloatingCombatTexts()
{
    if (!bShowFloatingCombatTexts)
    {
        return;
    }

    for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
    {
        AActor* Actor = *ActorIt;

        if (!IsValid(Actor))
        {
            return;
        }

        // Check floating combat texts.
        URTSFloatingCombatTextComponent* FloatingCombatTextComponent = Actor->FindComponentByClass<URTSFloatingCombatTextComponent>();

        if (!FloatingCombatTextComponent)
        {
            continue;
        }

        for (FRTSFloatingCombatTextData& TextData : FloatingCombatTextComponent->GetTexts())
        {
            // Calculate lifetime.
            float ElapsedLifetime = TextData.Lifetime - TextData.RemainingLifetime;
            float LifetimePercentage = ElapsedLifetime / TextData.Lifetime;

            // Calculate position.
            FVector2D Center = GetActorCenterOnScreen(Actor);
            FVector2D Size = GetActorSizeOnScreen(Actor);

            // Calculate color.
            FLinearColor TextColor = TextData.Color;

            if (bFadeOutFloatingCombatTexts)
            {
                TextColor.A = 1 - LifetimePercentage;
            }

            // Draw text.
            NotifyDrawFloatingCombatText(
                Actor,
                TextData.Text,
                TextColor,
                TextData.Scale,
                TextData.Lifetime,
                TextData.RemainingLifetime,
                LifetimePercentage,
                Center.X,
                Center.Y - (Size.Y / 2) - (FloatingCombatTextSpeed * ElapsedLifetime));
        }
    }
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
		for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;
			DrawHealthBar(Actor);
		}

		return;
	}

	// Draw health bar for hovered actor.
	if (bShowHoverHealthBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		DrawHealthBar(HoveredActor);
	}

	// Draw health bars for selected actors.
	if (bShowSelectionHealthBars)
	{
		for (int32 i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			DrawHealthBar(SelectedActor);
		}
	}
}

void ARTSHUD::DrawHealthBar(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	// Check health.
	URTSHealthComponent* HealthComponent = Actor->FindComponentByClass<URTSHealthComponent>();

	if (!HealthComponent)
	{
		return;
	}

	const float HealthPercentage = HealthComponent->GetCurrentHealth() / HealthComponent->GetMaximumHealth();

	// Suggest health bar size.
	float SuggestedHealthBarLeft;
	float SuggestedHealthBarTop;
	float SuggestedHealthBarWidth;
	float SuggestedHealthBarHeight;

	SuggestUnitBarSize(Actor, SuggestedHealthBarLeft, SuggestedHealthBarTop, SuggestedHealthBarWidth, SuggestedHealthBarHeight);

	// Draw health bar.
	NotifyDrawHealthBar(
		Actor,
		HealthComponent->GetCurrentHealth(),
		HealthComponent->GetMaximumHealth(),
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
		for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;
			DrawConstructionProgressBar(Actor);
		}

		return;
	}

	// Draw progress bar for hovered actor.
	if (bShowHoverConstructionProgressBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		DrawConstructionProgressBar(HoveredActor);
	}

	// Draw progress bars for selected actors.
	if (bShowSelectionConstructionProgressBars)
	{
		for (int32 i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			DrawConstructionProgressBar(SelectedActor);
		}
	}
}

void ARTSHUD::DrawConstructionProgressBar(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	// Check progress.
	URTSConstructionSiteComponent* ConstructionSiteComponent = Actor->FindComponentByClass<URTSConstructionSiteComponent>();

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
	
	SuggestUnitBarSize(Actor, SuggestedProgressBarLeft, SuggestedProgressBarTop, SuggestedProgressBarWidth, SuggestedProgressBarHeight);

	// Draw progress bar.
	NotifyDrawConstructionProgressBar(
		Actor,
		ConstructionSiteComponent->GetConstructionTime(),
		ConstructionSiteComponent->GetRemainingConstructionTime(),
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

void ARTSHUD::DrawProductionProgressBars()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	// Check override conditions.
	if (bAlwaysShowProductionProgressBars || (bShowHotkeyProductionProgressBars && PlayerController->IsProductionProgressBarHotkeyPressed()))
	{
		// Draw all progress bars.
		for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;
			DrawProductionProgressBar(Actor);
		}

		return;
	}

	// Draw progress bar for hovered actor.
	if (bShowHoverProductionProgressBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		DrawProductionProgressBar(HoveredActor);
	}

	// Draw progress bars for selected actors.
	if (bShowSelectionProductionProgressBars)
	{
		for (int32 i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			DrawProductionProgressBar(SelectedActor);
		}
	}
}

void ARTSHUD::DrawProductionProgressBar(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	// Check progress.
	URTSProductionComponent* ProductionComponent = Actor->FindComponentByClass<URTSProductionComponent>();

	if (!ProductionComponent)
	{
		return;
	}

	if (!ProductionComponent->IsProducing())
	{
		return;
	}

	const float ProgressPercentage = ProductionComponent->GetProgressPercentage();

	// Suggest progress bar size.
	float SuggestedProgressBarLeft;
	float SuggestedProgressBarTop;
	float SuggestedProgressBarWidth;
	float SuggestedProgressBarHeight;

	SuggestUnitBarSize(Actor, SuggestedProgressBarLeft, SuggestedProgressBarTop, SuggestedProgressBarWidth, SuggestedProgressBarHeight);

	// Draw progress bar.
	NotifyDrawProductionProgressBar(
		Actor,
		ProductionComponent->GetProductionTime(),
		ProductionComponent->GetRemainingProductionTime(),
		ProgressPercentage,
		SuggestedProgressBarLeft,
		SuggestedProgressBarTop,
		SuggestedProgressBarWidth,
		SuggestedProgressBarHeight);
}

void ARTSHUD::SuggestUnitBarSize(AActor* Actor, float& OutLeft, float& OutTop, float& OutWidth, float& OutHeight) const
{
	FVector2D Center = GetActorCenterOnScreen(Actor);
	FVector2D Size = GetActorSizeOnScreen(Actor);

	OutWidth = Size.X * 2;
	OutHeight = 15.0f;

	OutLeft = Center.X - (OutWidth / 2);
	OutTop = Center.Y - (Size.Y / 2) - (OutHeight * 2);
}
