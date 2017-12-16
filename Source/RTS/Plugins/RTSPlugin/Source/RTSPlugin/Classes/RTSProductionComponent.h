#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSProductionQueue.h"

#include "RTSProductionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSProductionComponentProductQueuedSignature, TSubclassOf<AActor>, ProductClass, int32, QueueIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductionStartedSignature, TSubclassOf<AActor>, ProductClass, int32, QueueIndex, float, TotalProductionTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSProductionComponentProductionFinishedSignature, AActor*, Product, int32, QueueIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSProductionComponentProductionCanceledSignature, TSubclassOf<AActor>, ProductClass, int32, QueueIndex, float, ElapsedProductionTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSProductionComponentProductionCostRefundedSignature, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/** Allows producing actors over time. */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSProductionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Types of actors the actor can produce. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> AvailableProducts;

	/** How many products may be queued per queue. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 1))
	int32 CapacityPerQueue;

	/** Products queued for production. */
	UPROPERTY(BlueprintReadOnly, Category = "RTS", replicated)
	TArray<FRTSProductionQueue> ProductionQueues;

	/** How many products can be produced simultaneously. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 1))
	int32 QueueCount;


	URTSProductionComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Checks whether the actor can start producing the specified product. */
	UFUNCTION(BlueprintCallable)
	virtual bool CanAssignProduction(TSubclassOf<AActor> ProductClass) const;

	/** Gets the index of the queue the specified product would be assigned to, or -1 all queues are at the capacity limit. */
	UFUNCTION(BlueprintCallable)
	virtual int32 FindQueueForProduct(TSubclassOf<AActor> ProductClass) const;

	/** Gets the product currently being produced in the specified queue. */
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetCurrentProduction(int32 QueueIndex = 0) const;

	/** Gets the required time for producing the current product in the specified queue. */
	UFUNCTION(BlueprintCallable)
	float GetProductionTime(int32 QueueIndex = 0) const;

	/** Gets the required time for producing the specified product. */
	UFUNCTION(BlueprintCallable)
	virtual float GetProductionTimeForProduct(TSubclassOf<AActor> ProductClass) const;

	/** Gets the current production progress [0..1] for the specified queue. */
	UFUNCTION(BlueprintCallable)
	float GetProgressPercentage(int32 QueueIndex = 0) const;

	/** Gets the time before producing the current product in the specified queue has finished. */
	UFUNCTION(BlueprintCallable)
	float GetRemainingProductionTime(int32 QueueIndex = 0) const;

	/** Whether any queue production timer is currently being ticked, or not. */
	UFUNCTION(BlueprintCallable)
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


	/** Event when a product has been queued for production. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductQueuedSignature OnProductQueued;

	/** Event when the production timer has been started. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSProductionComponentProductionStartedSignature OnProductionStarted;

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
	void DequeueProduct(int32 QueueIndex = 0, int32 ProductIndex = 0);
	void StartProductionInQueue(int32 QueueIndex = 0);
};
