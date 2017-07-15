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
	/** Material to draw when creating a selection frame by dragging the mouse. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	UMaterialInterface* SelectionFrameMaterial;


	virtual void DrawHUD() override;

	virtual void DrawHealthBar(ARTSCharacter* Character, float CurrentHealth, float MaximumHealth);


	/** Event for drawing an effect for the currently hovered actor. */
	virtual void NotifyDrawHoveredActorEffect(AActor* HoveredActor);

	/** Event for drawing an effect for the currently hovered actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "DrawHoveredActorEffect"))
	void ReceiveDrawHoveredActorEffect(AActor* HoveredActor);


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
