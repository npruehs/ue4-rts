#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

#include "RTSProductionQueue.generated.h"


USTRUCT(Category="RTS", BlueprintType)
struct REALTIMESTRATEGY_API FRTSProductionQueue
{
	GENERATED_BODY()

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
