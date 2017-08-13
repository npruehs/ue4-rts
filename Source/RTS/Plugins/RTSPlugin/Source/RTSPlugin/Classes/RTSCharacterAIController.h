#pragma once

#include "RTSPluginPrivatePCH.h"

#include "AIController.h"

#include "RTSOrderType.h"

#include "RTSCharacterAIController.generated.h"


class URTSAttackComponent;


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class ARTSCharacterAIController : public AAIController
{
    GENERATED_BODY()

public:
    /** Behavior tree to use for driving the character AI. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    UBehaviorTree* CharacterBehaviorTreeAsset;

    /** Blackboard to use for holding all data relevant to the character AI. */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RTS")
    UBlackboardData* CharacterBlackboardAsset;


	/** Makes the pawn look for a feasible target in its acquisition radius. */
	UFUNCTION(BlueprintCallable)
	void FindTargetInAcquisitionRadius();

	/** Makes the pawn attack the specified target. */
	UFUNCTION(BlueprintCallable)
	void IssueAttackOrder(AActor* Target);

	/** Makes the pawn construct the specified building at the passed location. */
	UFUNCTION(BlueprintCallable)
	void IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Makes the pawn continue constructing the specified building. */
	UFUNCTION(BlueprintCallable)
	void IssueContinueConstructionOrder(AActor* ConstructionSite);

    /** Makes the pawn move towards the specified location. */
    UFUNCTION(BlueprintCallable)
    void IssueMoveOrder(const FVector& Location);

	/** Makes the pawn stop all actions immediately. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();

protected:
	virtual void Possess(APawn* InPawn) override;

private:
	URTSAttackComponent* AttackComponent;

	void ApplyOrders();

	void ClearBuildingClass();
	void ClearHomeLocation();
	void ClearTargetActor();
	void ClearTargetLocation();

	void SetBuildingClass(int32 BuildingIndex);
	void SetHomeLocation(const FVector& HomeLocation);
	void SetOrderType(const ERTSOrderType OrderType);
	void SetTargetActor(AActor* TargetActor);
	void SetTargetLocation(const FVector& TargetLocation);

	bool TraceSphere(
		const FVector& Location,
		const float Radius,
		AActor* ActorToIgnore,
		ECollisionChannel TraceChannel,
		TArray<struct FHitResult>& OutHitResults);

	bool VerifyBlackboard();
};
