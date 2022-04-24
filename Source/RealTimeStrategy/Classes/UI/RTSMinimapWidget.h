#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Launch/Resources/Version.h"

#include "RTSMinimapWidget.generated.h"


class ARTSFogOfWarActor;
class ARTSMinimapVolume;
class ARTSPlayerController;
class ARTSVisionInfo;
class ARTSVisionVolume;


/**
* Widget for drawing a high-level overview of unit positions.
*/
UCLASS(Category="RTS", Blueprintable)
class REALTIMESTRATEGY_API URTSMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URTSMinimapWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Event for custom drawing of units on the minimap (e.g. for drawing hero portraits for hero units). */
	virtual void NotifyOnDrawUnit(
		FPaintContext& Context,
		AActor* Actor,
		APlayerState* ActorOwner,
		const FVector2D& MinimapPosition,
		APlayerState* LocalPlayer) const;

	/** Event for custom drawing of units on the minimap (e.g. for drawing hero portraits for hero units). */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnDrawUnit"))
	void ReceiveOnDrawUnit(
		UPARAM(ref) FPaintContext& Context,
		AActor* Actor,
		APlayerState* ActorOwner,
		const FVector2D& MinimapPosition,
		APlayerState* LocalPlayer) const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 20)
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
#else
	void NativePaint(FPaintContext& InContext) const override;
#endif

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	/** Brush for drawing the background of the current map. */
	UPROPERTY(EditAnywhere, Category = "RTS|Background")
	FSlateBrush MinimapBackground;

	/** Brush for drawing own units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	FSlateBrush OwnUnitsBrush;

	/** Brush for drawing enemy units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	FSlateBrush EnemyUnitsBrush;

	/** Brush for drawing neutral units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	FSlateBrush NeutralUnitsBrush;

	/** Brush for drawing neutral units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	FSlateBrush ResourcesBrush;
	/** Brush for drawing neutral units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	bool bOverrideResourceColor = true;

	/** Brush for drawing blinking damaged units on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	FSlateBrush DamagedUnitsBlinkBrush;

	/** Whether to draw the minimap background layer. */
	UPROPERTY(EditAnywhere, Category = "RTS|Background")
	bool bDrawBackground = true;

	/** Whether to draw unit dots on the minimap, with OwnUnitsBrush, EnemyUnitsBrush and NeutralUnitsBrush. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	bool bDrawUnitsWithTeamColors = true;

	/** Whether to draw vision on the minimap, with UnknownAreasBrush, KnownAreasBrush and VisibleAreasBrush. */
	UPROPERTY(EditAnywhere, Category = "RTS|Vision")
	bool bDrawVision = true;

	/** Whether to show the current camera frustum on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Camera")
	bool bDrawViewFrustum = true;

	/** Whether to show the current camera frustum on the minimap. */
	UPROPERTY(EditAnywhere, Category = "RTS|Camera")
	FLinearColor ViewFrustumTint = FLinearColor::White;

	/** How long to have a unit change its color on the minimap after it has taken damage. */
	UPROPERTY(EditAnywhere, Category = "RTS|Units")
	float DamagedUnitBlinkTimeSeconds;

	/** Material to instance for rendering the fog of war effect. */
	UPROPERTY(EditAnywhere, Category = "RTS|Vision", meta=(AllowPrivateAccess="true"))
	UMaterialInterface* FogOfWarMaterial;


	/** Provides visibility information. */
	UPROPERTY()
	ARTSFogOfWarActor* FogOfWarActor;

	/** User interface material instance for rendering fog of war on the minimap. */
	UPROPERTY()
	UMaterialInstanceDynamic* FogOfWarMaterialInstance;

	/** Brush for drawing fog of war on the minimap. */
	FSlateBrush FogOfWarBrush;

	bool bMouseDown;

	UPROPERTY()
	ARTSMinimapVolume* MinimapVolume;

	FVector MinimapWorldSize;

	UPROPERTY()
	ARTSVisionInfo* VisionInfo;

	UPROPERTY()
	ARTSVisionVolume* VisionVolume;

	void DrawBackground(FPaintContext& InContext) const;
	void DrawUnits(FPaintContext& InContext) const;
	void DrawVision(FPaintContext& InContext) const;
	void DrawViewFrustum(FPaintContext& InContext) const;

	void DrawBoxWithBrush(FPaintContext& InContext, const FVector2D& Position, const FSlateBrush& Brush) const;

	FReply HandleMinimapClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	FVector MinimapToWorld(const FVector2D& MinimapPosition) const;
	bool ViewportToWorld(ARTSPlayerController* Player, const FVector2D& ViewportPosition, FVector& OutWorldPosition) const;
	FVector2D WorldToMinimap(const FVector& WorldPosition) const;
};
