#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Templates/SubclassOf.h"

#include "RTSOrderType.h"

#include "RTSPawnAIController.generated.h"


class URTSAttackComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSPawnAIControllerOrderChangedSignature, AActor*, Actor, ERTSOrderType, NewOrder);


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSPawnAIController : public AAIController
{
    GENERATED_BODY()

public:
	/** Makes the pawn look for a feasible target in its acquisition radius. */
	UFUNCTION(BlueprintCallable)
	void FindTargetInAcquisitionRadius();

    /** Checks whether the pawn has an order of the specified type. */
    UFUNCTION(BlueprintPure)
    bool HasOrder(ERTSOrderType OrderType) const;

    /** Checks whether the pawn is idle, or has any orders. */
    UFUNCTION(BlueprintPure)
    bool IsIdle() const;

	/** Makes the pawn attack the specified target. */
	UFUNCTION(BlueprintCallable)
	void IssueAttackOrder(AActor* Target);

	/** Makes the pawn construct the specified building at the passed location. */
	UFUNCTION(BlueprintCallable)
	void IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Makes the pawn continue constructing the specified building. */
	UFUNCTION(BlueprintCallable)
	void IssueContinueConstructionOrder(AActor* ConstructionSite);

	/** Makes the pawn gather resources from the specified source. */
	UFUNCTION(BlueprintCallable)
	void IssueGatherOrder(AActor* ResourceSource);

    /** Makes the pawn move towards the specified location. */
    UFUNCTION(BlueprintCallable)
    void IssueMoveOrder(const FVector& Location);

	/** Makes the pawn move towards the closest resource drain and return all carried resources. */
	UFUNCTION(BlueprintCallable)
	void IssueReturnResourcesOrder();

	/** Makes the pawn stop all actions immediately. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();


    /** Event when the pawn has received a new order. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FRTSPawnAIControllerOrderChangedSignature OnOrderChanged;


protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
    /** Behavior tree to use for driving the pawn AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBehaviorTree* PawnBehaviorTreeAsset;

    /** Blackboard to use for holding all data relevant to the pawn AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBlackboardData* PawnBlackboardAsset;

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
