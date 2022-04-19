#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"

#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "Orders/RTSOrderGroupExecutionType.h"
#include "Orders/RTSOrderTargetData.h"

#include "RTSOrderLibrary.generated.h"


class AActor;


/**
* Utility functions for real-time strategy orders.
*/
UCLASS()
class REALTIMESTRATEGY_API URTSOrderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Checks whether the passed actor can obey the specified order. */
	static bool CanObeyOrder(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, int32 Index);

	/** Checks whether the specified actor and/or location is a valid target for the passed order. */
	static bool IsValidOrderTarget(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index);

	/** Issues the specified order to the passed actor. */
	static void IssueOrder(AActor* OrderedActor, const FRTSOrderData& Order);

	/** Issues the specified order to the passed actor. */
	static void IssueOrder(TSubclassOf<URTSOrder> OrderClass, AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index);

	/** Gets how many and which of the selected actors the specified order should be issued to. */
	static ERTSOrderGroupExecutionType GetOrderGroupExecutionType(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the normal icon of the specified order. */
	static UTexture2D* GetOrderNormalIcon(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the hovered icon of the specified order. */
	static UTexture2D* GetOrderHoveredIcon(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the pressed icon of the specified order. */
	static UTexture2D* GetOrderPressedIcon(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the disabled icon of the specified order. */
	static UTexture2D* GetOrderDisabledIcon(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the name of the specified order. */
	static FText GetOrderName(TSubclassOf<URTSOrder> OrderClass);

	/** Gets the description of the specified order. */
	static FText GetOrderDescription(TSubclassOf<URTSOrder> OrderClass, const AActor* OrderedActor, int32 Index);

	/** Gathers order target data, including relationship tags for the specified actors. */
	static FRTSOrderTargetData GetOrderTargetData(const AActor* OrderedActor, AActor* TargetActor, const FVector& TargetLocation = FVector::ZeroVector);

	/** Gets the gameplay tags of the specified actors, including relationship tags. */
	static void GetSourceAndTargetTags(const AActor* SourceActor, const AActor* TargetActor,
	                                   FGameplayTagContainer& OutSourceTags, FGameplayTagContainer& OutTargetTags);
};
