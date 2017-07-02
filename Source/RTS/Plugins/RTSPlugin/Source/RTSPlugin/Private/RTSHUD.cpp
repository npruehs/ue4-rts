#include "RTSPluginPrivatePCH.h"
#include "RTSHUD.h"

#include "RTSPlayerController.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelectionFrame();
}

void ARTSHUD::DrawSelectionFrame()
{
	if (!SelectionFrameMaterial)
	{
		return;
	}

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

	// Draw texture.
	DrawMaterialSimple(
		SelectionFrameMaterial,
		SelectionFrame.Min.X,
		SelectionFrame.Min.Y,
		SelectionFrame.Width(),
		SelectionFrame.Height());
}
