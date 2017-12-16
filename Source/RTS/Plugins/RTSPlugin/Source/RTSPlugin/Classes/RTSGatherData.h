#pragma once

#include "RTSPluginPCH.h"

#include "RTSGatherData.generated.h"


class URTSResourceType;


USTRUCT(BlueprintType)
struct RTSPLUGIN_API FRTSGatherData
{
	GENERATED_USTRUCT_BODY()

public:
	/** Type of resources that can be gathered. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TSubclassOf<class URTSResourceType> ResourceType;

	/** Amount of resources gathered after each cooldown. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float AmountPerGathering;

	/** Maximum amount of resources that can be carried. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Capacity;

	/** Time between two gatherings, in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Cooldown;

	/** Whether the actor needs to return to the drain in order to drop resources. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	bool bNeedsReturnToDrain;

	/** Range in which resources can be gathered, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Range;
};
