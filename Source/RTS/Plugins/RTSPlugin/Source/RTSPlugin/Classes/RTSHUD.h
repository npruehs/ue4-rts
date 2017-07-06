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

private:
	/** Draws the current selection frame if mouse is being dragged. */
	void DrawSelectionFrame();

	/** Draws unit health bars. */
	void DrawHealthBars();
};
