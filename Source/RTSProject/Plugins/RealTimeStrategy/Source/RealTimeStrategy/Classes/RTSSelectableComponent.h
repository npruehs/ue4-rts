#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSSelectableComponent.generated.h"


class UMaterialInterface;
class UMaterialInstanceDynamic;
class USoundCue;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentDeselectedSignature);


/**
 * Allows selecting the actor, e.g. by left-click.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;


	/** Selects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void SelectActor();

	/** Deselects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void DeselectActor();

	/** Checks whether the unit is currently selected by the local player, or not. */
	UFUNCTION(BlueprintPure)
	bool IsSelected() const;

    /** Gets the sound to play when the actor is selected. */
    USoundCue* GetSelectedSound() const;

	/** Event when the actor has been deselected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentDeselectedSignature OnDeselected;

	/** Event when the actor has been selected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentSelectedSignature OnSelected;


private:
    /** Material for rendering the selection circle of the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UMaterialInterface* SelectionCircleMaterial;

    /** Sound to play when the actor is selected. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    USoundCue* SelectedSound;

	/** Whether the unit is currently selected by the local player, or not. */
	bool bSelected;

    /** Decal used for rendering the selection circle of the actor. */
    UPROPERTY()
    UDecalComponent* DecalComponent;

    /** Material instance for rendering the selection circle of the actor. */
    UPROPERTY()
    UMaterialInstanceDynamic* SelectionCircleMaterialInstance;
};
