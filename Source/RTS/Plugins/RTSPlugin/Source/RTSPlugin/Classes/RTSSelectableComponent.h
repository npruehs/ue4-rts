#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSSelectableComponent.generated.h"


class UMaterialInterface;
class UMaterialInstanceDynamic;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentDeselectedSignature);


/**
 * Allows selecting the actor, e.g. by left-click.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    /** Material for rendering the selection circle of the actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    UMaterialInterface* SelectionCircleMaterial;


    virtual void BeginPlay() override;


	/** Selects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void SelectActor();

	/** Deselects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
	void DeselectActor();

	/** Checks whether the unit is currently selected by the local player, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsSelected();


	/** Event when the actor has been deselected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentDeselectedSignature OnDeselected;

	/** Event when the actor has been selected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSSelectableComponentSelectedSignature OnSelected;


private:
	/** Whether the unit is currently selected by the local player, or not. */
	bool bSelected;

    /** Decal used for rendering the selection circle of this character. */
    UDecalComponent* DecalComponent;

    /** Material instance for rendering the selection circle of the actor. */
    UMaterialInstanceDynamic* SelectionCircleMaterialInstance;
};
