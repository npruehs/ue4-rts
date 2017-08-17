#pragma once

#include "RTSPluginPrivatePCH.h"

#include "RTSProductionCostType.generated.h"


UENUM(BlueprintType)
enum class ERTSProductionCostType : uint8
{
	/** Pay whole production cost as soon as production starts. */
	COST_PayImmediately,

	/** Pay production costs every tick while production proceeds. */
	COST_PayOverTime,

	/** Don't pay production costs automatically. */
	COST_PayCustom
};
