#include "UI/RTSHUD.h"

#include "EngineUtils.h"
#include "Components/ShapeComponent.h"

#include "RTSPlayerController.h"
#include "Combat/RTSHealthComponent.h"
#include "Combat/RTSHealthBarWidgetComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Construction/RTSConstructionProgressBarWidgetComponent.h"
#include "Production/RTSProductionComponent.h"
#include "UI/RTSFloatingCombatTextComponent.h"
#include "UI/RTSFloatingCombatTextData.h"
#include "UI/RTSHoveredActorWidgetComponent.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

    DrawFloatingCombatTexts();
	DrawSelectionFrame();
	DrawHealthBars();
	DrawConstructionProgressBars();
	DrawProductionProgressBars();
	DrawHoveredActorWidget();
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

void ARTSHUD::NotifyHideSelectionFrame()
{
    ReceiveHideSelectionFrame();
}

FVector2D ARTSHUD::GetActorCenterOnScreen(AActor* Actor) const
{
	FVector ProjectedLocation = Project(Actor->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetActorSizeOnScreen(AActor* Actor) const
{
	// Get actor position projected on HUD.
    UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();

	if (!ShapeComponent)
	{
        return FVector2D::ZeroVector;
	}

    FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

    FVector ActorTopPosition;
    FVector ActorBottomPosition;
    FVector ActorLeftPosition;
    FVector ActorRightPosition;

    if (CollisionShape.IsCapsule())
    {
        ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
        ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
        ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));
        ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));
    }
    else if (CollisionShape.IsBox())
    {
        ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Box.HalfExtentX));
        ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Box.HalfExtentX));
        ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Box.HalfExtentY));
        ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Box.HalfExtentY));
    }
    else if (CollisionShape.IsSphere())
    {
        ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Sphere.Radius));
        ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Sphere.Radius));
        ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Sphere.Radius));
        ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Sphere.Radius));
    }
    else
    {
        return FVector2D::ZeroVector;
    }

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
        if (bWasDrawingSelectionFrame)
        {
            NotifyHideSelectionFrame();
        }
        
        bWasDrawingSelectionFrame = false;
		return;
	}

	// Draw selection frame.
	NotifyDrawSelectionFrame(
		SelectionFrame.Min.X,
		SelectionFrame.Min.Y,
		SelectionFrame.Width(),
		SelectionFrame.Height());

    bWasDrawingSelectionFrame = true;
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

    for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
    {
        AActor* Actor = *ActorIt;
       
        // Check override conditions.
        if (bAlwaysShowHealthBars || (bShowHotkeyHealthBars && PlayerController->IsHealthBarHotkeyPressed()))
        {
            // Draw all health bars.
            DrawHealthBar(Actor);
        }
        else if (bShowHoverHealthBars && Actor == PlayerController->GetHoveredActor())
        {
            // Draw health bar for hovered actor.
            DrawHealthBar(Actor);
        }
        else if (bShowSelectionHealthBars && PlayerController->GetSelectedActors().Contains(Actor))
        {
            // Draw health bars for selected actors.
            DrawHealthBar(Actor);
        }
        else
        {
            HideHealthBar(Actor);
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

	if (!IsValid(HealthComponent))
	{
		return;
	}

	const float HealthPercentage = HealthComponent->GetCurrentHealth() / HealthComponent->GetMaximumHealth();

    // Draw health bar.
    URTSHealthBarWidgetComponent* HealthBarWidgetComponent = Actor->FindComponentByClass<URTSHealthBarWidgetComponent>();

    if (!IsValid(HealthBarWidgetComponent))
    {
        return;
    }

    FVector2D Size = GetActorSizeOnScreen(Actor);

    HealthBarWidgetComponent->UpdatePositionAndSize(Size);
    HealthBarWidgetComponent->SetVisibility(true);
}

void ARTSHUD::HideHealthBar(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return;
    }

    URTSHealthBarWidgetComponent* HealthBarWidgetComponent = Actor->FindComponentByClass<URTSHealthBarWidgetComponent>();

    if (!IsValid(HealthBarWidgetComponent))
    {
        return;
    }

    HealthBarWidgetComponent->SetVisibility(false);
}

void ARTSHUD::DrawConstructionProgressBars()
{
    ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

    if (!PlayerController)
    {
        return;
    }

    for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
    {
        AActor* Actor = *ActorIt;

        // Check override conditions.
        if (bAlwaysShowConstructionProgressBars || (bShowHotkeyConstructionProgressBars && PlayerController->IsConstructionProgressBarHotkeyPressed()))
        {
            // Draw all progress bars.
            DrawConstructionProgressBar(Actor);
        }
        else if (bShowHoverConstructionProgressBars && Actor == PlayerController->GetHoveredActor())
        {
            // Draw progress bar for hovered actor.
            DrawConstructionProgressBar(Actor);
        }
        else if (bShowSelectionConstructionProgressBars && PlayerController->GetSelectedActors().Contains(Actor))
        {
            // Draw progress bars for selected actors.
            DrawConstructionProgressBar(Actor);
        }
        else
        {
            HideConstructionProgressBar(Actor);
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

    // Draw progress bar.
    URTSConstructionProgressBarWidgetComponent* ConstructionProgressBarWidgetComponent =
        Actor->FindComponentByClass<URTSConstructionProgressBarWidgetComponent>();

    if (!IsValid(ConstructionProgressBarWidgetComponent))
    {
        return;
    }

    FVector2D Size = GetActorSizeOnScreen(Actor);

    ConstructionProgressBarWidgetComponent->UpdatePositionAndSize(Size);
    ConstructionProgressBarWidgetComponent->SetVisibility(true);
}

void ARTSHUD::HideConstructionProgressBar(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return;
    }

    URTSConstructionProgressBarWidgetComponent* ConstructionProgressBarWidgetComponent =
        Actor->FindComponentByClass<URTSConstructionProgressBarWidgetComponent>();

    if (!IsValid(ConstructionProgressBarWidgetComponent))
    {
        return;
    }

    ConstructionProgressBarWidgetComponent->SetVisibility(false);
}

void ARTSHUD::DrawHoveredActorWidget()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	AActor* NewHoveredActor = PlayerController->GetHoveredActor();

	if (NewHoveredActor == OldHoveredActor)
	{
		return;
	}

    if (IsValid(OldHoveredActor))
    {
        URTSHoveredActorWidgetComponent* OldHoveredActorWidgetComponent =
            OldHoveredActor->FindComponentByClass<URTSHoveredActorWidgetComponent>();

        if (IsValid(OldHoveredActorWidgetComponent))
        {
            OldHoveredActorWidgetComponent->SetVisibility(false);
        }
    }
    
    if (IsValid(NewHoveredActor))
    {
        URTSHoveredActorWidgetComponent* NewHoveredActorWidgetComponent =
            NewHoveredActor->FindComponentByClass<URTSHoveredActorWidgetComponent>();

        if (IsValid(NewHoveredActorWidgetComponent))
        {
            FVector2D Size = GetActorSizeOnScreen(NewHoveredActor);

            NewHoveredActorWidgetComponent->UpdatePositionAndSize(Size);
            NewHoveredActorWidgetComponent->UpdateData(NewHoveredActor);
            NewHoveredActorWidgetComponent->SetVisibility(true);
        }
    }

    OldHoveredActor = NewHoveredActor;
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
