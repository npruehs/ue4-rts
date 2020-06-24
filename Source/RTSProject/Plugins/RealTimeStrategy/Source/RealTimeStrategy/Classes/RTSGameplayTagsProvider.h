#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "RTSGameplayTagsProvider.generated.h"

/** Provides initial gameplay tags for actors. */
UINTERFACE(MinimalAPI)
class URTSGameplayTagsProvider : public UInterface
{
    GENERATED_BODY()
};

/** Provides initial gameplay tags for actors. */
class REALTIMESTRATEGY_API IRTSGameplayTagsProvider
{
    GENERATED_BODY()

public:
    /** Adds gameplay tags to the specified tag container. */
    virtual void AddGameplayTags(FGameplayTagContainer& InOutTagContainer);
};
