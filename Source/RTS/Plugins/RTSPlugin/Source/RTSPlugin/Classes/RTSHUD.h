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
	/** Whether to always show all health bars. You may expose this in your own Options window: Some professional players might want this for perfect micro-management. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Health Bars")
	bool bAlwaysShowHealthBars;

	/** Whether to show health bars for hovered units. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Health Bars")
	bool bShowHoverHealthBars = true;

	/** Whether to show health bars for selected units. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Health Bars")
	bool bShowSelectionHealthBars = true;

	/** Whether to show health bars while the respective hotkey is pressed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Health Bars")
	bool bShowHotkeyHealthBars = true;


	virtual void DrawHUD() override;


	/** Event for drawing an effect for the currently hovered actor. */
	virtual void NotifyDrawHoveredActorEffect(AActor* HoveredActor);

	/** Event for drawing the health bar for the specified character. */
	virtual void NotifyDrawHealthBar(
		ARTSCharacter* Character,
		float CurrentHealth,
		float MaximumHealth,
		float HealthPercentage,
		float SuggestedHealthBarLeft,
		float SuggestedHealthBarTop,
		float SuggestedHealthBarWidth,
		float SuggestedHealthBarHeight);

	/** Event for drawing the selection frame because the mouse is being dragged. */
	virtual void NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height);

	/** Event for drawing an effect for the currently hovered actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "DrawHoveredActorEffect"))
	void ReceiveDrawHoveredActorEffect(AActor* HoveredActor);

	/** Event for drawing the health bar for the specified character. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "DrawHealthBar"))
	void ReceiveDrawHealthBar(
		ARTSCharacter* Character,
		float CurrentHealth,
		float MaximumHealth,
		float HealthPercentage,
		float SuggestedHealthBarLeft,
		float SuggestedHealthBarTop,
		float SuggestedHealthBarWidth,
		float SuggestedHealthBarHeigh);

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

	/** Draws the health bar of the specified character. */
	void DrawHealthBar(ARTSCharacter* Character);

	/** Draws a custom HUD effect for the currently hovered actor (e.g. player name). */
	void DrawHoveredActorEffect();
};
