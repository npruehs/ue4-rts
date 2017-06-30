#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/PlayerStart.h"

#include "RTSPlayerStart.generated.h"


/**
* Player start with RTS features, such as which player has been spawned at this start.
*/
UCLASS()
class ARTSPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	/** Player who's been spawned at this start. */
	AController* Player;
};
