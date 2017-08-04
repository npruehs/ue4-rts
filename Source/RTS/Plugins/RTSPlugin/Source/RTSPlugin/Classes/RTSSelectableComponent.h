#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSSelectableComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentDeselectedSignature);


/**
 * Allows selecting the actor, e.g. by left-click.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Selects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void SelectActor();

	/** Deselects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void DeselectActor();

	/** Checks whether the unit is currently selected by the local player, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsSelected();


	/** Event when the character has been deselected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentDeselectedSignature OnDeselected;

	/** Event when the character has been selected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentSelectedSignature OnSelected;


private:
	/** Whether the unit is currently selected by the local player, or not. */
	bool bSelected;
};
