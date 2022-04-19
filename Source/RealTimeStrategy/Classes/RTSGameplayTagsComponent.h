#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

#include "RTSGameplayTagsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSGameplayTagsComponentCurrentTagsChangedSignature, AActor*, Actor, FGameplayTagContainer, CurrentTags);


/**
* Allows tagging an actor with various properties.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSGameplayTagsComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	URTSGameplayTagsComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	/** Gets the current set of gameplay tags of the actor. */
	FGameplayTagContainer GetCurrentTags() const;

	/** Adds the specified gameplay tag to the actor. */
	UFUNCTION(BlueprintCallable)
	void AddGameplayTag(const FGameplayTag& NewTag);

	/** Adds the specified gameplay tags to the actor. */
	UFUNCTION(BlueprintCallable)
	void AddGameplayTags(const FGameplayTagContainer& NewTags);

	/** Removes the specified gameplay tag from the actor. */
	UFUNCTION(BlueprintCallable)
	bool RemoveGameplayTag(const FGameplayTag& TagToRemove);

	/** Removes the specified gameplay tags from the actor. */
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTags(const FGameplayTagContainer& TagsToRemove);


	/** Event when the current gameplay tags of the actor have changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSGameplayTagsComponentCurrentTagsChangedSignature CurrentTagsChanged;

private:
	/** Tags to add on BeginPlay. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FGameplayTagContainer InitialTags;

	/** Current set of gameplay tags of the actor. */
	UPROPERTY(ReplicatedUsing = ReceivedCurrentTags)
	FGameplayTagContainer CurrentTags;

	/** Event when the current gameplay tags of the actor have changed. */
	virtual void NotifyOnCurrentTagsChanged();

	UFUNCTION()
	void ReceivedCurrentTags();
};
