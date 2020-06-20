#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"

#include "RTSGameplayTagLibrary.generated.h"

/**
* Utility functions for adding, checking and removing gameplay tags.
*/
UCLASS()
class REALTIMESTRATEGY_API URTSGameplayTagLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Applies the passed gameplay tag to the specified actor. */
    UFUNCTION(BlueprintCallable, Category = "RTS")
    static void AddGameplayTag(const AActor* Actor, const FGameplayTag& Tag);

    /** Checks whether the specified actor currently has the passed gameplay tag applied. */
    UFUNCTION(BlueprintPure, Category = "RTS")
    static bool HasGameplayTag(const AActor* Actor, const FGameplayTag& Tag);

    /** Removes the passed gameplay tag from the specified actor. */
    UFUNCTION(BlueprintCallable, Category = "RTS")
    static void RemoveGameplayTag(const AActor* Actor, const FGameplayTag& Tag);

    /** Actor can contain builders. */
    static const FGameplayTag& Container_ConstructionSite();

    /** Actor can contain gatherers. */
    static const FGameplayTag& Container_ResourceSource();

    /** Actor is hidden because of being in a container. */
    static const FGameplayTag& HideReason_Container();

    /** Actor is friendly towards the other actor. */
    static const FGameplayTag& Relationship_Friendly();

    /** Actor is owned by the same player as the other actor. */
    static const FGameplayTag& Relationship_SamePlayer();

    /** Actor is visible for the other actor. */
    static const FGameplayTag& Relationship_Visible();

    /** Actor has positive health. */
    static const FGameplayTag& Status_Changing_Alive();

    /** Actor is currently working at a construction site. */
    static const FGameplayTag& Status_Changing_Constructing();

    /** Actor temporarily cannot move. */
    static const FGameplayTag& Status_Changing_Immobilized();

    /** Actor cannot be attacked or targeted. */
    static const FGameplayTag& Status_Changing_Invulnerable();

    /** Actor temporarily cannot attack other actors. */
    static const FGameplayTag& Status_Changing_Unarmed();

    /** Actor is still being constructed by builders. */
    static const FGameplayTag& Status_Changing_UnderConstruction();

    /** Actor can attack other actors. */
    static const FGameplayTag& Status_Permanent_CanAttack();

    /** Actor can be attacked by other actors. */
    static const FGameplayTag& Status_Permanent_CanBeAttacked();

    /** Actor can construct buildings. */
    static const FGameplayTag& Status_Permanent_CanConstruct();

    /** Actor can be constructed by builders. */
    static const FGameplayTag& Status_Permanent_CanBeConstructed();

    /** Actor can gather resources from other actors. */
    static const FGameplayTag& Status_Permanent_CanGather();

    /** Actor provides resources for other actors. */
    static const FGameplayTag& Status_Permanent_CanBeGathered();

    /** Actor can produce other actors. */
    static const FGameplayTag& Status_Permanent_CanProduce();
};
