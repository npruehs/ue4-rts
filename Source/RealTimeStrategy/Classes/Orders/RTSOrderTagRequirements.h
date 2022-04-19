#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "RTSOrderTagRequirements.generated.h"


/** Tag requirements for ordered actors and order targets. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSOrderTagRequirements
{
	GENERATED_BODY()

public:
	/** Gameplay tags required by the ordered actor. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FGameplayTagContainer SourceRequiredTags;

	/** Gameplay tags the ordered actor is not allowed to have. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FGameplayTagContainer SourceBlockedTags;

	/** Gameplay tags required by the target actor. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FGameplayTagContainer TargetRequiredTags;

	/** Gameplay tags the target actor is not allowed to have. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FGameplayTagContainer TargetBlockedTags;
};
