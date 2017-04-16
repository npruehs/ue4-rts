#pragma once

#include "RTSPluginPrivatePCH.h"

#include "AIController.h"

#include "RTSCharacterAIController.generated.h"


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class ARTSCharacterAIController : public AAIController
{
    GENERATED_BODY()

public:
    /** Blackboard to use for holding all data relevant to the character AI. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    UBlackboardData* CharacterBlackboardAsset;

protected:
    virtual void BeginPlay() override;

};