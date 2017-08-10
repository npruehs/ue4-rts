#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/PlayerController.h"

#include "RTSPlayerController.generated.h"


class USkeletalMesh;

class ARTSBuildingCursor;
class ARTSCameraBoundsVolume;
class ARTSPlayerState;


/**
 * Player controller with RTS features, such as selection and mouse camera movement.
 */
UCLASS()
class ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    /** Movement speed of the camera when moved with keys or mouse, in cm/sec. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
    float CameraSpeed;

    /** Distance from the screen border at which the mouse cursor causes the camera to move, in pixels. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
	int32 CameraScrollThreshold;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Construction")
	TSubclassOf<ARTSBuildingCursor> BuildingCursorClass;


	virtual void PlayerTick(float DeltaTime) override;


	/** Gets the actor currently hovered by this player. */
	UFUNCTION(BlueprintCallable)
	AActor* GetHoveredActor();

	/** Gets the replicated state of this player. */
	UFUNCTION(BlueprintCallable)
	ARTSPlayerState* GetPlayerState();

	/** Gets the list of units currently selected by this player. */
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetSelectedActors();

	/** Casts a ray from the specified screen position and collects the results. */
	bool GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults);

	/** Casts a ray to find any objects at the specified world position. */
	bool GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults);

	/** Gets the current selection frame, in screen space. */
	bool GetSelectionFrame(FIntRect& OutSelectionFrame);


	/** Orders all selected units to attack the specified unit. */
	UFUNCTION(BlueprintCallable)
	bool IssueAttackOrder(AActor* Target);

	/** Orders a selected builder to construct the specified building at the passed location. */
	UFUNCTION(BlueprintCallable)
	bool IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Orders selected builders to finish constructing the specified building. */
	UFUNCTION(BlueprintCallable)
	bool IssueContinueConstructionOrder(AActor* ConstructionSite);

	/** Orders all selected units to move to the specified location. */
	UFUNCTION(BlueprintCallable)
	bool IssueMoveOrder(const FVector& TargetLocation);

	/** Orders all selected units to stop all current actions. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();

	/** Selects the specified actors. */
	UFUNCTION(BlueprintCallable)
	void SelectActors(TArray<AActor*> Actors);

	/** Saves the current selection to the specified control group. */
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable) void SaveControlGroup0();
	UFUNCTION(BlueprintCallable) void SaveControlGroup1();
	UFUNCTION(BlueprintCallable) void SaveControlGroup2();
	UFUNCTION(BlueprintCallable) void SaveControlGroup3();
	UFUNCTION(BlueprintCallable) void SaveControlGroup4();
	UFUNCTION(BlueprintCallable) void SaveControlGroup5();
	UFUNCTION(BlueprintCallable) void SaveControlGroup6();
	UFUNCTION(BlueprintCallable) void SaveControlGroup7();
	UFUNCTION(BlueprintCallable) void SaveControlGroup8();
	UFUNCTION(BlueprintCallable) void SaveControlGroup9();

	/** Restores the selection saved in the specified control group. */
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable) void LoadControlGroup0();
	UFUNCTION(BlueprintCallable) void LoadControlGroup1();
	UFUNCTION(BlueprintCallable) void LoadControlGroup2();
	UFUNCTION(BlueprintCallable) void LoadControlGroup3();
	UFUNCTION(BlueprintCallable) void LoadControlGroup4();
	UFUNCTION(BlueprintCallable) void LoadControlGroup5();
	UFUNCTION(BlueprintCallable) void LoadControlGroup6();
	UFUNCTION(BlueprintCallable) void LoadControlGroup7();
	UFUNCTION(BlueprintCallable) void LoadControlGroup8();
	UFUNCTION(BlueprintCallable) void LoadControlGroup9();

	/** Whether the hotkey for showing all construction progress bars is currently pressed, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsConstructionProgressBarHotkeyPressed();

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsHealthBarHotkeyPressed();

	/** Whether the hotkey for showing all production progress bars is currently pressed, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsProductionProgressBarHotkeyPressed();

	/** Begin finding a suitable location for constructing a building of the specified type. */
	UFUNCTION(BlueprintCallable)
	void BeginBuildingPlacement(TSubclassOf<AActor> BuildingType);

	/**
	 * Checks whether the specified building can be placed at the passed location.
	 * Default implementation checks for any dynamic objects within a box of the specified collision size.
	 * You may add custom building placement logic here, e.g. requires other nearby building, cursed terrain, energy field, fixed slot.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "RTS", meta = (DisplayName = "CanPlaceBuilding"))
	bool CanPlaceBuilding(TSubclassOf<AActor> BuildingType, const FVector& Location) const;
	virtual bool CanPlaceBuilding_Implementation(TSubclassOf<AActor> BuildingType, const FVector& Location) const;


	/** Event when this player is now owning the specified actor. */
	virtual void NotifyOnActorOwnerChanged(AActor* Actor);

	/** Event when the player begins placing a building. */
	virtual void NotifyOnBuildingPlacementStarted(TSubclassOf<AActor> BuildingType);

	/** Event when the player confirms a location for placing a building. */
	virtual void NotifyOnBuildingPlacementConfirmed(TSubclassOf<AActor> BuildingType, const FVector& Location);

	/** Event when the player receives an error placing a building at a specific location. */
	virtual void NotifyOnBuildingPlacementError(TSubclassOf<AActor> BuildingType, const FVector& Location);

	/** Event when the player cancels placing a building. */
	virtual void NotifyOnBuildingPlacementCancelled(TSubclassOf<AActor> BuildingType);

	/** Event when an actor has received an attack order. */
	virtual void NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Event when an actor has received a begin construction order. */
	virtual void NotifyOnIssuedBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Event when an actor has received a continue construction order. */
	virtual void NotifyOnIssuedContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite);

    /** Event when an actor has received a move order. */
    virtual void NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	virtual void NotifyOnIssuedStopOrder(APawn* OrderedPawn);

	/** Event when the player has clicked a spot on the minimap. */
	virtual void NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition);

    /** Event when the set of selected actors of this player has changed. */
    virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when this player is now owning the specified actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Ownership", meta = (DisplayName = "OnActorOwnerChanged"))
	void ReceiveOnActorOwnerChanged(AActor* Actor);

	/** Event when the player begins placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementStarted"))
	void ReceiveOnBuildingPlacementStarted(TSubclassOf<AActor> BuildingType);

	/** Event when the player confirms a location for placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementConfirmed"))
	void ReceiveOnBuildingPlacementConfirmed(TSubclassOf<AActor> BuildingType, const FVector& Location);

	/** Event when the player receives an error placing a building at a specific location. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementError"))
	void ReceiveOnBuildingPlacementError(TSubclassOf<AActor> BuildingType, const FVector& Location);

	/** Event when the player cancels placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementCancelled"))
	void ReceiveOnBuildingPlacementCancelled(TSubclassOf<AActor> BuildingType);

	/** Event when an actor has received an attack order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedAttackOrder"))
	void ReceiveOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Event when an actor has received a begin construction order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedBeginConstructionOrder"))
	void ReceiveOnIssuedBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Event when an actor has received a continue construction order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedContinueConstructionOrder"))
	void ReceiveOnIssuedContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite);

    /** Event when an actor has received a move order. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedMoveOrder"))
    void ReceiveOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedStopOrder"))
	void ReceiveOnIssuedStopOrder(APawn* OrderedPawn);

	/** Event when the player has clicked a spot on the minimap. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Minimap", meta = (DisplayName = "NotifyOnMinimapClicked"))
	void ReceiveOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition);

    /** Event when the set of selected actors of this player has changed. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Selection", meta = (DisplayName = "OnSelectionChanged"))
    void ReceiveOnSelectionChanged(const TArray<AActor*>& Selection);


	/** Sets this player as the owner of the specified actor. */
	UFUNCTION(BlueprintCallable)
	void TransferOwnership(AActor* Actor);


protected:
    virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
    /** Volume that restricts the camera movement of this player. */
    ARTSCameraBoundsVolume* CameraBoundsVolume;

    /** Last horizontal axis input applied to camera movement. */
    float CameraLeftRightAxisValue;

    /** Last vertical axis input applied to camera movement. */
    float CameraUpDownAxisValue;


	/** Saved selections of this player. */
	TArray<TArray<AActor*>> ControlGroups;

	/** Actor currently hovered by this player. */
	AActor* HoveredActor;

	/** World position currently hovered by this player. */
	FVector HoveredWorldPosition;

    /** Actors selected by this player. */
    TArray<AActor*> SelectedActors;

	/** Type of the building currently being placed, if any. */
	TSubclassOf<AActor> BuildingBeingPlacedType;

	/** Current cursor for placing a new building. */
	ARTSBuildingCursor* BuildingCursor;


	/** Whether we're currently creating a selection frame by dragging the mouse. */
	bool bCreatingSelectionFrame;

	/** Mouse position on screen when creating the selection frame started. */
	FVector2D SelectionFrameMouseStartPosition;

	/** Whether the hotkey for showing all construction progress bars is currently pressed, or not. */
	bool bConstructionProgressBarHotkeyPressed;

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	bool bHealthBarHotkeyPressed;

	/** Whether the hotkey for showing all production progress bars is currently pressed, or not. */
	bool bProductionProgressBarHotkeyPressed;

	/** Whether to add clicked units to the current selection. */
	bool bAddSelectionHotkeyPressed;

	/** Whether to add clicked units to the current selection, if they're not already selected, and deselect them otherwise. */
	bool bToggleSelectionHotkeyPressed;


    /** Casts a ray from the current mouse position and collects the results. */
    bool GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults);
	
	/** Casts a box from the current selection frame and collects the results. */
	bool GetObjectsInSelectionFrame(TArray<FHitResult>& OutHitResults);

	/** Traces all relevant objects using the specified ray. */
	bool TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults);

	/** Checks whether the specified actor is valid and selectable. */
	bool IsSelectableActor(AActor* Actor);

    /** Automatically issues the most reasonable order for the current pointer position. */
    UFUNCTION()
    void IssueOrder();

	/** Automatically issues the most reasonable order for the specified targets. */
	void IssueOrderTargetingObjects(TArray<FHitResult>& HitResults);

	/** Cancels constructing the specified actor, destroying the construction site. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCancelConstruction(AActor* ConstructionSite);

	/** Orders the passed unit to attack the specified unit. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueAttackOrder(APawn* OrderedPawn, AActor* Target);
	
	/** Orders a selected builder to construct the specified building at the passed location. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingType, const FVector& TargetLocation);

	/** Orders selected builders to finish constructing the specified building. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite);

	/** Orders the passed unit to move to the specified location. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Orders the passed unit to stop all current actions. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueStopOrder(APawn* OrderedPawn);

	/** Start producing the specified product at the specified actor. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartProduction(AActor* ProductionActor, int32 ProductIndex);

	/** Cancels the current production at the specified actor. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCancelProduction(AActor* ProductionActor);

    /** Applies horizontal axis input to camera movement. */
    void MoveCameraLeftRight(float Value);

    /** Applies vertical axis input to camera movement. */
    void MoveCameraUpDown(float Value);

    /** Remembers the current mouse position for multi-selection, finished by FinishSelectActors. */
    UFUNCTION()
    void StartSelectActors();

	/** Selects all selectable actors within the created selection frame, started by StartSelectActors. */
	UFUNCTION()
	void FinishSelectActors();

	/** Force showing all construction progress bars. */
	UFUNCTION()
	void StartShowingConstructionProgressBars();

	/** Stop showing all construction progress bars. */
	UFUNCTION()
	void StopShowingConstructionProgressBars();

	/** Force showing all health bars. */
	UFUNCTION()
	void StartShowingHealthBars();

	/** Stop showing all health bars. */
	UFUNCTION()
	void StopShowingHealthBars();

	/** Force showing all production progress bars. */
	UFUNCTION()
	void StartShowingProductionProgressBars();

	/** Stop showing all production progress bars. */
	UFUNCTION()
	void StopShowingProductionProgressBars();

	/** Start adding clicked units to the current selection. */
	UFUNCTION()
	void StartAddSelection();

	/** Stop adding clicked units to the current selection. */
	UFUNCTION()
	void StopAddSelection();

	/** Start adding clicked units to the current selection, if they're not already selected, and deselecting them otherwise. */
	UFUNCTION()
	void StartToggleSelection();

	/** Stop adding clicked units to the current selection, if they're not already selected, and deselecting them otherwise. */
	UFUNCTION()
	void StopToggleSelection();
	
	/** Begin placing the first building available to the currently selected builder. */
	UFUNCTION()
	void BeginDefaultBuildingPlacement();

	/** Confirms placing the current building at the hovered location. */
	UFUNCTION()
	void ConfirmBuildingPlacement();

	/** Cancels placing the current building without effect. */
	UFUNCTION()
	void CancelBuildingPlacement();

	/** Cancels the construction of the first selected building. */
	UFUNCTION()
	void CancelConstruction();

	/** Start producing the first product available to the currently selected production actor. */
	UFUNCTION()
	void StartDefaultProduction();

	/** Cancels the current production of the first selected building. */
	UFUNCTION()
	void CancelProduction();
};
