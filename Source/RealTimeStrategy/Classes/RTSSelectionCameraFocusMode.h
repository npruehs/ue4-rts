#pragma once

#include "CoreMinimal.h"

#include "RTSSelectionCameraFocusMode.generated.h"

/** How to handle camera focus for selected actors. */
UENUM(Category="RTS", BlueprintType)
enum class ERTSSelectionCameraFocusMode : uint8
{
	/** Do nothing. */
	SELECTIONFOCUS_DoNothing,

	/** Focus the camera on the selected actor if it's rapidly selected twice. */
	SELECTIONFOCUS_FocusOnDoubleSelection,
};
