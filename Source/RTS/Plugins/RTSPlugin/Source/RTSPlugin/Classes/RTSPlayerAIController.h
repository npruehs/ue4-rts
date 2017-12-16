#pragma once

#include "RTSPluginPCH.h"

#include "AIController.h"

#include "RTSPlayerAIController.generated.h"


class UBehaviorTree;
class UBlackboardData;
class APawn;

class URTSPlayerResourcesComponent;


/**
* AI controller that drives strategic RTS player AI.
*/
UCLASS()
class RTSPLUGIN_API ARTSPlayerAIController : public AAIController
{
    GENERATED_BODY()

public:
    ARTSPlayerAIController();

    /** Behavior tree to use for driving the player AI. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    UBehaviorTree* PlayerBehaviorTreeAsset;

    /** Blackboard to use for holding all data relevant to the player AI. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    UBlackboardData* PlayerBlackboardAsset;

    /** Units and buildings the AI should produce, in order. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    TArray<TSubclassOf<APawn>> BuildOrder;

    /** Maximum distance of a new building to an existing one, in cm. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    float MaximumBaseBuildingDistance;


    /** Checks the build order and returns the class of the next pawn to produce, or the Pawn class if nothing needs to be produced. */
    UFUNCTION(BlueprintPure)
    TSubclassOf<APawn> GetNextPawnToProduce() const;

    /** Checks the available resources for the AI and whether it can pay for a pawn of the specified class. */
    UFUNCTION(BlueprintPure)
    bool CanPayFor(TSubclassOf<APawn> PawnClass) const;

    /** Selects an arbitrary production actor for producing a pawn of the specified class and starts production. */
    UFUNCTION(BlueprintCallable)
    bool StartProduction(TSubclassOf<APawn> PawnClass);

protected:
    virtual void Possess(APawn* InPawn) override;


private:
    /** Stores the resources available for this player. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    URTSPlayerResourcesComponent* PlayerResourcesComponent;
};
