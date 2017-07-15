#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/HUD.h"

#include "RTSHUD.generated.h"


class ARTSCharacter;


/**
* HUD with RTS features, such as showing a selection frame.
*/
UCLASS()
class ARTSHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	virtual void DrawHealthBar(ARTSCharacter* Character, float CurrentHealth, float MaximumHealth);


	/** Event for drawing an effect for the currently hovered actor. */
	virtual void NotifyDrawHoveredActorEffect(AActor* HoveredActor);

	/** Event for drawing the selection frame because the mouse is being dragged. */
	virtual void NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height);

	/** Event for drawing an effect for the currently hovered actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "DrawHoveredActorEffect"))
	void ReceiveDrawHoveredActorEffect(AActor* HoveredActor);

	/** Event for drawing the selection frame because the mouse is being dragged. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "DrawSelectionFrame"))
	void ReceiveDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height);


	UFUNCTION(BlueprintCallable)
	FVector2D GetCharacterCenterOnScreen(ACharacter* Character);

	UFUNCTION(BlueprintCallable)
	FVector2D GetCharacterSizeOnScreen(ACharacter* Character);

private:
	/** Draws the current selection frame if mouse is being dragged. */
	void DrawSelectionFrame();

	/** Draws unit health bars. */
	void DrawHealthBars();
};
