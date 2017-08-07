#pragma once

#include "RTSPluginPrivatePCH.h"

#include "RTSConstructionState.generated.h"


UENUM(BlueprintType)
enum class ERTSConstructionState : uint8
{
	/** Construction has not yet started. */
	CONSTRUCTIONSTATE_NotStarted,

	/** Building is being constructed right now. */
	CONSTRUCTIONSTATE_Constructing,

	/** Construction timer is paused. */
	CONSTRUCTIONSTATE_Paused,

	/** Construction has finished. */
	CONSTRUCTIONSTATE_Finished
};
