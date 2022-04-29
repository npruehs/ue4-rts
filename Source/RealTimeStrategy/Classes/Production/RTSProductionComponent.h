#pragma once

#include "CoreMinimal.h"

#include "RTSActorComponent.h"

#include "Templates/SubclassOf.h"

#include "Production/RTSProductionQueue.h"
#include "Production/RTSProductionRallyPoint.h"

#include "RTSProductionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductQueuedSignature, AActor*, Actor, TSubclassOf<AActor>, ProductClass, int32, QueueIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSProductionComponentProductionStartedSignature, AActor*, Actor, TSubclassOf<AActor>, ProductClass, int32, QueueIndex, float, TotalProductionTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductionProgressChangedSignature, AActor*, Actor, int32, QueueIndex, float, ProgressPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductionFinishedSignature, AActor*, Actor, AActor*, Product, int32, QueueIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSProductionComponentProductionCanceledSignature, AActor*, Actor, TSubclassOf<AActor>, ProductClass, int32, QueueIndex, float, ElapsedProductionTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductionCostRefundedSignature, AActor*, Actor, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/** Allows producing actors over time. */
UCLASS(ClassGroup="RTS", Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSProductionComponent : public URTSActorComponent
{
	GENERATED_BODY()

public:
	URTSProductionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/** Checks whether the actor can start producing the specified product. */
	UFUNCTION(BlueprintPure)
	virtual bool CanAssignProduction(TSubclassOf<AActor> ProductClass) const;

	/** Gets the index of the queue the specified product would be assigned to, or -1 all queues are at the capacity limit. */
	UFUNCTION(BlueprintPure)
	virtual int32 FindQueueForProduct(TSubclassOf<AActor> ProductClass) const;

	/** Gets the product currently being produced in the specified queue. */
	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetCurrentProduction(int32 QueueIndex = 0) const;

	/** Gets the required time for producing the current product in the specified queue. */
	UFUNCTION(BlueprintPure)
	float GetProductionTime(int32 QueueIndex = 0) const;

	/** Gets the required time for producing the specified product. */
	UFUNCTION(BlueprintPure)
	virtual float GetProductionTimeForProduct(TSubclassOf<AActor> ProductClass) const;

	/** Gets the current production progress [0..1] for the specified queue. */
	UFUNCTION(BlueprintPure)
	float GetProgressPercentage(int32 QueueIndex = 0) const;

	/** Gets the time before producing the current product in the specified queue has finished. */
	UFUNCTION(BlueprintPure)
	float GetRemainingProductionTime(int32 QueueIndex = 0) const;

	/** Whether any queue production timer is currently being ticked, or not. */
	UFUNCTION(BlueprintPure)
	bool IsProducing() const;

	/** Starts producing the specified product, setting the timer. */
	UFUNCTION(BlueprintCallable)
	virtual void StartProduction(TSubclassOf<AActor> ProductClass);

	/** Finishes producing the product in the specified queue. */
	UFUNCTION(BlueprintCallable)
	virtual void FinishProduction(int32 QueueIndex = 0);

	/** Cancels producing the product in the specified queue. */
	UFUNCTION(BlueprintCallable)
	virtual void CancelProduction(int32 QueueIndex = 0, int32 ProductIndex = 0);

	/** Sets the rally point on the specified actor. */
	void SetRallyPointToActor(AActor* Target);

	/** Sets the rally point to the specified actor. */
	void SetRallyPointToLocation(const FVector& TargetLocation);

	/** Clears the rally point. */
	void ClearRallyPoint();


	/** Gets the types of actors the actor can produce. */
	UFUNCTION(BlueprintPure)
	TArray<TSubclassOf<AActor>> GetAvailableProducts() const;

	/** Gets how many products can be produced simultaneously. */
	UFUNCTION(BlueprintPure)
	int32 GetQueueCount() const;

	/** Gets how many products may be queued per queue. */
	UFUNCTION(BlueprintPure)
	int32 GetCapacityPerQueue() const;

	/** Gets the actor or location to send new actors to. */
	UFUNCTION(BlueprintPure)
	FRTSProductionRallyPoint GetRallyPoint() const;


	/** Event when the production timer has expired. */
	virtual void NotifyOnProductionFinished(AActor* Actor, AActor* Product, int32 QueueIndex);


	/** Event when a product has been queued for production. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductQueuedSignature OnProductQueued;

	/** Event when the production timer has been started. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionStartedSignature OnProductionStarted;

	/** Event when the production timer has been updated. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionProgressChangedSignature OnProductionProgressChanged;

	/** Event when the production timer has expired. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionFinishedSignature OnProductionFinished;

	/** Event when the production has been canceled. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionCanceledSignature OnProductionCanceled;

	/** Event when any production costs have been refunded. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionCostRefundedSignature OnProductionCostRefunded;

private:
	/** Types of actors the actor can produce. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TSubclassOf<AActor>> AvailableProducts;

	/** How many products can be produced simultaneously. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 1))
	int32 QueueCount;

	/** How many products may be queued per queue. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 1))
	int32 CapacityPerQueue;

	/** Products queued for production. */
	UPROPERTY(ReplicatedUsing=ReceivedProductionQueues)
	TArray<FRTSProductionQueue> ProductionQueues;

	/** Product that the actor finished most recently. */
	UPROPERTY(ReplicatedUsing=ReceivedMostRecentProduct)
	AActor* MostRecentProduct;

	/** Actor or location to send new actors to. */
	UPROPERTY(Replicated)
	FRTSProductionRallyPoint RallyPoint;

	void DequeueProduct(int32 QueueIndex = 0, int32 ProductIndex = 0);
	void StartProductionInQueue(int32 QueueIndex = 0);

	void IssueRallyPointDependentOrder(AActor* ProducedActor);

	UFUNCTION()
	void ReceivedProductionQueues();

	UFUNCTION()
	void ReceivedMostRecentProduct();
};
