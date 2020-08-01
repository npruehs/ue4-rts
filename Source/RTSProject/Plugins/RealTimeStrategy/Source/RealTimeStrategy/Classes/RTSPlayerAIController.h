#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Templates/SubclassOf.h"

#include "Economy/RTSResourceType.h"

#include "RTSPlayerAIController.generated.h"


class UBehaviorTree;
class UBlackboardData;
class APawn;

class URTSPlayerResourcesComponent;


/**
* AI controller that drives strategic RTS player AI.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSPlayerAIController : public AAIController
{
    GENERATED_BODY()

public:
    ARTSPlayerAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    /** Checks the build order and returns the class of the next pawn to produce, or the Pawn class if nothing needs to be produced. */
    UFUNCTION(BlueprintPure)
    TSubclassOf<APawn> GetNextPawnToProduce() const;

    /** Gets the primary resource drain of the AI. */
    UFUNCTION(BlueprintPure)
    virtual AActor* GetPrimaryResourceDrain() const;

    /** Gets the closest resource source of the primary resource type around the first resource drain of the AI. */
    UFUNCTION(BlueprintPure)
    virtual AActor* GetPrimaryResourceSource() const;

    /** Checks the available resources for the AI and whether it can pay for a pawn of the specified class. */
    UFUNCTION(BlueprintPure)
    bool CanPayFor(TSubclassOf<APawn> PawnClass) const;

    /** Checks whether the AI meets all tech tree requirements for a pawn of the specified class. */
    UFUNCTION(BlueprintPure)
    bool MeetsAllRequirementsFor(TSubclassOf<APawn> PawnClass) const;

    /** Selects an arbitrary production actor for producing a pawn of the specified class and starts production. */
    UFUNCTION(BlueprintCallable)
    bool StartProduction(TSubclassOf<APawn> PawnClass);

    /** Whether killing an actor owned by this player yields a reward for the attacking player. */
    bool GivesBounty() const;

protected:
    virtual void OnPossess(APawn* InPawn) override;


private:
    /** Behavior tree to use for driving the player AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBehaviorTree* PlayerBehaviorTreeAsset;

    /** Blackboard to use for holding all data relevant to the player AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBlackboardData* PlayerBlackboardAsset;

    /** Units and buildings the AI should produce, in order. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<TSubclassOf<APawn>> BuildOrder;

    /** Maximum distance of a new building to an existing one, in cm. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float MaximumBaseBuildingDistance;

    /** Type of the primary resource for the AI to gather (e.g. used for placing resource drains). */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TSubclassOf<URTSResourceType> PrimaryResourceType;

    /** Whether killing an actor owned by this player yields a reward for the attacking player. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bGivesBounty;

    /** Stores the resources available for this player. */
    UPROPERTY(VisibleAnywhere, Category = "RTS")
    URTSPlayerResourcesComponent* PlayerResourcesComponent;
};
