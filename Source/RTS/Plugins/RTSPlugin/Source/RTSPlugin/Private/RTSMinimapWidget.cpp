#include "RTSPluginPrivatePCH.h"
#include "RTSMinimapWidget.h"

#include "Components/BrushComponent.h"
#include "GameFramework/PlayerController.h"

#include "RTSMinimapVolume.h"


URTSMinimapWidget::URTSMinimapWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URTSMinimapWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	// Get minimap bounds.
	for (TActorIterator<ARTSMinimapVolume> It(GetWorld()); It; ++It)
	{
		MinimapVolume = *It;
		break;
	}

	if (!MinimapVolume)
	{
		UE_LOG(RTSLog, Warning, TEXT("No RTSMinimapVolume found. Minimap won't be showing unit positions."));
		return;
	}

	UBrushComponent* MinimapBrushComponent = MinimapVolume->GetBrushComponent();
	FBoxSphereBounds MinimapBounds = MinimapBrushComponent->CalcBounds(MinimapBrushComponent->ComponentToWorld);

	MinimapWorldSize = MinimapBounds.BoxExtent * 2;
}

void URTSMinimapWidget::NativePaint(FPaintContext& InContext) const
{
	UUserWidget::NativePaint(InContext);

	InContext.MaxLayer++;

	DrawBackground(InContext);
	DrawUnits(InContext);
}

void URTSMinimapWidget::DrawBackground(FPaintContext& InContext) const
{
	DrawBoxWithBrush(InContext, FVector2D(0, 0), MinimapBackground);
}

void URTSMinimapWidget::DrawUnits(FPaintContext& InContext) const
{
	if (!MinimapVolume)
	{
		return;
	}

	APlayerController* Player = GetOwningPlayer();

	for (TActorIterator<ARTSCharacter> CharacterIt(GetWorld()); CharacterIt; ++CharacterIt)
	{
		ARTSCharacter* Character = *CharacterIt;

		// Get relative world position.
		FVector CharacterLocation = Character->GetActorLocation();

		float RelativeWorldX = CharacterLocation.X / MinimapWorldSize.X + 0.5f;
		float RelativeWorldY = CharacterLocation.Y / MinimapWorldSize.Y + 0.5f;

		// Rotate to match UI coordinate system.
		float temp = RelativeWorldX;
		RelativeWorldX = RelativeWorldY;
		RelativeWorldY = 1 - temp;

		// Convert to minimap coordinates.
		float MinimapX = RelativeWorldX * MinimapBackground.ImageSize.X;
		float MinimapY = RelativeWorldY * MinimapBackground.ImageSize.Y;

		// Draw on minimap.
		if (Character->GetPlayerOwner() == Player->PlayerState)
		{
			DrawBoxWithBrush(InContext, FVector2D(MinimapX, MinimapY), OwnUnitsBrush);
		}
		else if (Character->GetPlayerOwner() != nullptr)
		{
			DrawBoxWithBrush(InContext, FVector2D(MinimapX, MinimapY), EnemyUnitsBrush);
		}
		else
		{
			DrawBoxWithBrush(InContext, FVector2D(MinimapX, MinimapY), NeutralUnitsBrush);
		}
	}
}

void URTSMinimapWidget::DrawBoxWithBrush(FPaintContext& InContext, const FVector2D& Position, const FSlateBrush& Brush) const
{
	FSlateDrawElement::MakeBox(
		InContext.OutDrawElements,
		InContext.MaxLayer,
		InContext.AllottedGeometry.ToPaintGeometry(Position, Brush.ImageSize),
		&Brush,
		InContext.MyClippingRect,
		ESlateDrawEffect::None,
		Brush.TintColor.GetSpecifiedColor());
}
