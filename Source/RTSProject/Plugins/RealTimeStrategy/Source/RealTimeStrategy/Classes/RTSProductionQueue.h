#pragma once

#include "RealTimeStrategyPCH.h"

#include "RTSProductionQueue.generated.h"


USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSProductionQueue
{
	GENERATED_USTRUCT_BODY()

public:
	/** Products queued for production. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> Queue;

	/** Time before the current actor in the queue is produced, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float RemainingProductionTime;


	/** Gets the product with the specified index in this queue. */
	TSubclassOf<AActor> operator[](int32 Index) const;


	/** Enqueues the specified product. */
	void Add(TSubclassOf<AActor> Product);

	/** Gets the number of products in this queue. */
	int32 Num() const;

	/** Removes the product with the specified index in this queue, advancing all later products. */
	void RemoveAt(int32 Index);
};
