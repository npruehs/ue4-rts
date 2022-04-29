#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Templates/SubclassOf.h"

#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "RTSOrderType.h"

#include "RTSPawnAIController.generated.h"


class URTSAttackComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSPawnAIControllerOrderChangedSignature, AActor*, Actor, ERTSOrderType, NewOrder);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSPawnAIControllerCurrentOrderChangedSignature, AActor*, Actor, const FRTSOrderData&, NewOrder);


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API ARTSPawnAIController : public AAIController
{
	GENERATED_BODY()

public:
	/** Makes the pawn look for a feasible target in its acquisition radius. */
	UFUNCTION(BlueprintCallable)
	void FindTargetInAcquisitionRadius();

	/** Gets the class of the order currently issued to the pawn. */
	UFUNCTION(BlueprintPure)
	TSubclassOf<URTSOrder> GetCurrentOrder() const;

	/** Issues the specified order to the passed pawn. */
	UFUNCTION(Reliable, Server)
	void ServerAddOrder(const FRTSOrderData& Order);

	UFUNCTION(BlueprintCallable)
	void AddOrder(const FRTSOrderData& Order);

	UFUNCTION(BlueprintCallable)
	void InsertOrder(const FRTSOrderData& Order);

	/** Checks whether the pawn has an order of the specified type. */
	UFUNCTION(BlueprintPure)
	bool HasOrder(ERTSOrderType OrderType) const;

	/** Checks whether the pawn has an order of the specified type. */
	UFUNCTION(BlueprintPure)
	bool HasOrderByClass(TSubclassOf<URTSOrder> OrderClass) const;

	/** Checks whether the pawn is idle, or has any orders. */
	UFUNCTION(BlueprintPure)
	bool IsIdle() const;

	/** Makes the pawn carry out the specified order. */
	UFUNCTION(BlueprintCallable)
	void IssueOrder(const FRTSOrderData& Order);

	UFUNCTION(Reliable, Server)
	void ServerIssueOrder(const FRTSOrderData& Order);

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
	
	UFUNCTION(BlueprintCallable)
	void InsertContinueGathersOrder();

	/** Makes the pawn move towards the specified location. */
	UFUNCTION(BlueprintCallable)
	void IssueMoveOrder(const FVector& Location);

	/** Makes the pawn move towards the closest resource drain and return all carried resources. */
	UFUNCTION(BlueprintCallable)
	void IssueReturnResourcesOrder();

	/** Makes the pawn move towards the closest resource drain and return all carried resources. */
	UFUNCTION(BlueprintCallable)
	void InsertReturnResourcesOrder();

	/** Makes the pawn stop all actions immediately. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();
	
	UFUNCTION(BlueprintCallable)
	void ObtainNextOrder();


	/** Deprecated as of plugin version 1.2. Please use OnCurrentOrderChanged instead. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSPawnAIControllerOrderChangedSignature OnOrderChanged;

	/** Event when the pawn has received a new order. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSPawnAIControllerCurrentOrderChangedSignature OnCurrentOrderChanged;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	TQueue<FRTSOrderData> Orders;

private:
	/** Behavior tree to use for driving the pawn AI. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	UBehaviorTree* PawnBehaviorTreeAsset;

	/** Blackboard to use for holding all data relevant to the pawn AI. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	UBlackboardData* PawnBlackboardAsset;

	/** Types of actors to filter for when trying to find targets in the acquisition radius. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TEnumAsByte<EObjectTypeQuery>> AcquisitionObjectTypes;
	
    UPROPERTY()
	URTSAttackComponent* AttackComponent;

	ERTSOrderType OrderClassToType(UClass* OrderClass) const;
};
