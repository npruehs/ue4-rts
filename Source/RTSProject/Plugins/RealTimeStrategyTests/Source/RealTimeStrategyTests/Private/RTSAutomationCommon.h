#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

class AActor;


/**
* Utility functions for automating tests.
*/
class REALTIMESTRATEGYTESTS_API FRTSAutomationCommon
{
public:
    /** Gets the first actor with the specified tag. */
    static AActor* GetActorWithTag(const FName& ActorTag);

    /** Latent. Loads the specified map and starts a new Play In Editor session. */
	static void LatentLoadMapAndStartPIE(const FString& MapName);
};


/** Tests whether the actor with the specified tag is at the passed expected location, ignoring Z. */
DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FRTSTestActorLocationXYCommand, FName, ActorTag, FVector, ExpectedLocation);
