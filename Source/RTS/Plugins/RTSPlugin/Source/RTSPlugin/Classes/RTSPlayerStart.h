#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/PlayerStart.h"

#include "RTSPlayerStart.generated.h"


/**
* Player start with RTS features, such as which player has been spawned at this start.
*/
UCLASS()
class RTSPLUGIN_API ARTSPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	/** Team to add the spawned player to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int32 TeamIndex;

	/** Player who's been spawned at this start. */
	AController* Player;
};
