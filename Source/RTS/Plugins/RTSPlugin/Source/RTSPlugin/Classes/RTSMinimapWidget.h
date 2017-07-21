#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Blueprint/UserWidget.h"

#include "RTSMinimapWidget.generated.h"


class ARTSMinimapVolume;
class ARTSPlayerController;


/**
* Widget for drawing a high-level overview of unit positions.
*/
UCLASS(Blueprintable)
class URTSMinimapWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

public:
	/** Brush for drawing the background of the current map. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	FSlateBrush MinimapBackground;

	/** Brush for drawing own units on the minimap. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	FSlateBrush OwnUnitsBrush;

	/** Brush for drawing enemy units on the minimap. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	FSlateBrush EnemyUnitsBrush;

	/** Brush for drawing neutral units on the minimap. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	FSlateBrush NeutralUnitsBrush;


protected:
	void NativeConstruct() override;
	void NativePaint(FPaintContext& InContext) const override;


private:
	ARTSMinimapVolume* MinimapVolume;
	FVector MinimapWorldSize;

	void DrawBackground(FPaintContext& InContext) const;
	void DrawUnits(FPaintContext& InContext) const;
	void DrawViewFrustum(FPaintContext& InContext) const;

	void DrawBoxWithBrush(FPaintContext& InContext, const FVector2D& Position, const FSlateBrush& Brush) const;

	bool ViewportToWorld(ARTSPlayerController* Player, const FVector2D& ViewportPosition, FVector& WorldPosition) const;
	FVector2D WorldToMinimap(const FVector& WorldPosition) const;
};
