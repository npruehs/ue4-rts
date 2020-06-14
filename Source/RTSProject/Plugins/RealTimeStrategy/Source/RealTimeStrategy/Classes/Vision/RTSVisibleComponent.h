#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Vision/RTSVisionState.h"

#include "RTSVisibleComponent.generated.h"


class AActor;


/** Allows the actor to be hidden by fog of war or other effects. */
UCLASS(meta=(BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSVisibleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    /** Client. Override the visibility of the actor, forcing it to be visible or hidden because of the specified reason. */
    UFUNCTION(BlueprintCallable)
    void SetClientHideReason(const FGameplayTag& HideReason, bool bHidden);

    /** Client. Remembers whether the actor is hidden due to vision for the local player. */
    void SetClientVisionState(ERTSVisionState InVisionState);

private:
    /** Prevent the actor from ever being hidden again after seen once (e.g. buildings). */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bDontHideAfterSeen;

    /** Whether the actor and all children are currently visible, or not. */
    bool bVisible;

    /** Client. Whether the actor is hidden due to vision for the local player. */
    ERTSVisionState ClientVisionState;

    /** Client. Whether the local player has ever seen the actor. */
    bool bClientWasEverSeen;

    /** Reasons for which the actor is currently hidden. */
    FGameplayTagContainer HideReasonTags;

    UFUNCTION()
    void OnContainerChanged(AActor* Actor, AActor* NewContainer);

    /** Client. Shows or hides the actor locally, based on all effects that can hide actors (e.g. containers, fog of war). */
    void UpdateClientHiddenFlag();

    /** Client. Whether the actor is owned by a player that is in the same team as the local player. */
    bool IsSameTeamAsLocalClient() const;

    /** Client. Whether the actor is visible due to vision for the local player. */
    bool IsVisibleForLocalClient() const;

    /** Sets the actor and all of its components visible or invisible. */
    void SetVisible(bool bInVisible);
};
