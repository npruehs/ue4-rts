#pragma once

#include "RTSPluginPrivatePCH.h"

#include "RTSOrderType.generated.h"


UENUM(BlueprintType)
enum ERTSOrderType
{
	/** Idle. */
	ORDER_None,

	/** Move to a destination in the world. */
	ORDER_Move,

	/** Attack another actor. */
	ORDER_Attack
};