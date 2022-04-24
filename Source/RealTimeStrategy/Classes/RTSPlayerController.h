#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "Templates/SubclassOf.h"

#include "RTSControlGroup.h"
#include "RTSSelectionCameraFocusMode.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"

#include "RTSPlayerController.generated.h"


class USkeletalMesh;

class ARTSBuildingCursor;
class ARTSCameraBoundsVolume;
class URTSPlayerAdvantageComponent;
class URTSPlayerResourcesComponent;
class ARTSPlayerState;
class URTSResourceType;
class ARTSTeamInfo;
class ARTSVisionInfo;


/**
 * Player controller with RTS features, such as selection and mouse camera movement.
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PlayerTick(float DeltaTime) override;

	virtual void InitPlayerState() override;
	virtual void OnRep_PlayerState() override;

	/** Gets the actor currently hovered by this player. */
	UFUNCTION(BlueprintPure)
	AActor* GetHoveredActor() const;

	/** Gets the replicated state of this player. */
	UFUNCTION(BlueprintPure)
	ARTSPlayerState* GetPlayerState() const;

	/** Gets the list of units currently selected by this player. */
	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetSelectedActors() const;

	/** Casts a ray from the specified screen position and collects the results. */
	bool GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults) const;

	/** Casts a ray to find any objects at the specified world position. */
	bool GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults) const;

	/** Gets the current selection frame, in screen space. */
	bool GetSelectionFrame(FIntRect& OutSelectionFrame) const;

	/** Gets the team this player belongs to. */
	UFUNCTION(BlueprintPure)
	ARTSTeamInfo* GetTeamInfo() const;

	/** Issues the specified order to all selected units. */
	UFUNCTION(BlueprintCallable)
	bool IssueOrderToSelectedActors(const FRTSOrderData& Order);

	/** Issues the first successful default order for the specified targets to the passed actor. */
	void IssueDefaultOrderToActor(AActor* Actor, AActor* TargetActor, const FVector& TargetLocation);

	/** Orders all selected units to attack the specified unit. */
	UFUNCTION(BlueprintCallable)
	bool IssueAttackOrder(AActor* Target);

	/** Orders a selected builder to construct the specified building at the passed location. */
	UFUNCTION(BlueprintCallable)
	bool IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Orders selected builders to finish constructing the specified building. */
	UFUNCTION(BlueprintCallable)
	bool IssueContinueConstructionOrder(AActor* ConstructionSite);

	/** Orders selected gatherers to gather resources from the specified source. */
	UFUNCTION(BlueprintCallable)
	bool IssueGatherOrder(AActor* ResourceSource);

	/** Orders all selected units to move to the specified location. */
	UFUNCTION(BlueprintCallable)
	bool IssueMoveOrder(const FVector& TargetLocation);

	/** Gets a selected actor suitable for production. */
	AActor* GetSelectedProductionActorFor(TSubclassOf<AActor> ProductClass) const;

	/** Checks whether this player can begin producing the product with the specified index (e.g. can pay for it), and shows an error message otherwise. */
	UFUNCTION(BlueprintPure)
	bool CheckCanIssueProductionOrder(TSubclassOf<AActor> ProductClass);

	/** Orders the selected production actor to start producing the product with the specified index. */
	UFUNCTION(BlueprintCallable)
	void IssueProductionOrder(TSubclassOf<AActor> ProductClass);

	/** Orders all selected units to stop all current actions. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();

	/** Selects the specified actors. */
	UFUNCTION(BlueprintCallable)
	void SelectActors(TArray<AActor*> Actors, ERTSSelectionCameraFocusMode CameraFocusMode);

	/** Gets the currently selected subgroup of selected actors. */
	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetSelectedSubgroup() const;

	/** Gets any member of the currently selected subgroup of selected actors. */
	UFUNCTION(BlueprintPure)
	AActor* GetSelectedSubgroupActor();

	/** Gets all members of the currently selected subgroup of selected actors. */
	UFUNCTION(BlueprintPure)
	void GetSelectedSubgroupActors(TArray<AActor*>& OutActors) const;

	/** Selects the first subgroup of selected actors. */
	UFUNCTION(BlueprintCallable)
	void SelectFirstSubgroup();

	/** Selects the next subgroup of selected actors, wrapping around. */
	UFUNCTION(BlueprintCallable)
	void SelectNextSubgroup();

	/** Selects the previous subgroup of selected actors, wrapping around. */
	UFUNCTION(BlueprintCallable)
	void SelectPreviousSubgroup();

	/** Selects the specified subgroup of selected actors. */
	UFUNCTION(Category = RTS, BlueprintCallable)
	void SelectSubgroup(TSubclassOf<AActor> NewSubgroup);

	/** Saves the current selection to the specified control group. */
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SaveControlGroup0();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup1();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup2();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup3();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup4();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup5();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup6();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup7();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup8();
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup9();

	/** Restores the selection saved in the specified control group. */
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable)
	void LoadControlGroup0();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup1();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup2();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup3();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup4();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup5();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup6();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup7();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup8();
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup9();

	/** Makes the camera focus the specified world location. */
	UFUNCTION(BlueprintCallable)
	void FocusCameraOnLocation(FVector2D NewCameraLocation);

	/** Makes the camera focus the specified actor. */
	UFUNCTION(BlueprintCallable)
	void FocusCameraOnActor(AActor* Actor);

	/** Makes the camera focus the specified actors. */
	UFUNCTION(BlueprintCallable)
	void FocusCameraOnActors(TArray<AActor*> Actors);

	/** Saves the current camera location to the specified index. */
	UFUNCTION(BlueprintCallable)
	void SaveCameraLocation(int32 Index);

	/** Saves the current camera location to the specified index. */
	template <int32 Index>
	void SaveCameraLocationWithIndex()
	{
		SaveCameraLocation(Index);
	}

	/** Restores the camera location with the specified index. */
	UFUNCTION(BlueprintCallable)
	void LoadCameraLocation(int32 Index);

	/** Restores the camera location with the specified index. */
	template <int32 Index>
	void LoadCameraLocationWithIndex()
	{
		LoadCameraLocation(Index);
	}

	/** Whether the hotkey for showing all construction progress bars is currently pressed, or not. */
	UFUNCTION(BlueprintPure)
	bool IsConstructionProgressBarHotkeyPressed() const;

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	UFUNCTION(BlueprintPure)
	bool IsHealthBarHotkeyPressed() const;

	/** Whether the hotkey for showing all production progress bars is currently pressed, or not. */
	UFUNCTION(BlueprintPure)
	bool IsProductionProgressBarHotkeyPressed() const;

	/** Checks whether this player can begin placing a building of the specified class (e.g. can pay for it), and shows an error message otherwise. */
	UFUNCTION(BlueprintPure)
	bool CheckCanBeginBuildingPlacement(TSubclassOf<AActor> BuildingClass);

	/** Begin finding a suitable location for constructing a building of the specified type. */
	UFUNCTION(BlueprintCallable)
	void BeginBuildingPlacement(TSubclassOf<AActor> BuildingClass);

	/**
	 * Checks whether the specified building can be placed at the passed location.
	 * Default implementation checks for any dynamic objects within a box of the specified collision size.
	 * You may add custom building placement logic here, e.g. requires other nearby building, cursed terrain, energy field, fixed slot.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "RTS", meta = (DisplayName = "CanPlaceBuilding"))
	bool CanPlaceBuilding(TSubclassOf<AActor> BuildingClass, const FVector& Location) const;
	virtual bool CanPlaceBuilding_Implementation(TSubclassOf<AActor> BuildingClass, const FVector& Location) const;

	/** Surrenders the current game. */
	UFUNCTION(BlueprintCallable)
	void Surrender();

	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;

	/** Notifies this client that the game has ended. */
	UFUNCTION(Reliable, Client)
	virtual void ClientGameHasEnded(bool bIsWinner);


	/** Event when this player is now owning the specified actor. */
	virtual void NotifyOnActorOwnerChanged(AActor* Actor);

	/** Event when the player begins placing a building. */
	virtual void NotifyOnBuildingPlacementStarted(TSubclassOf<AActor> BuildingClass);

	/** Event when the player confirms a location for placing a building. */
	virtual void NotifyOnBuildingPlacementConfirmed(TSubclassOf<AActor> BuildingClass, const FVector& Location);

	/** Event when the player receives an error placing a building at a specific location. */
	virtual void NotifyOnBuildingPlacementError(TSubclassOf<AActor> BuildingClass, const FVector& Location);

	/** Event when the player cancels placing a building. */
	virtual void NotifyOnBuildingPlacementCancelled(TSubclassOf<AActor> BuildingClass);

	/** Event when an error has occurred that can be presented to the user. */
	virtual void NotifyOnErrorOccurred(const FString& ErrorMessage);

	/** Event when the game has ended. */
	virtual void NotifyOnGameHasEnded(bool bIsWinner);

	/** Event when a pawn has received an order. */
	virtual void NotifyOnIssuedOrder(APawn* OrderedPawn, const FRTSOrderData& Order);

	/** Event when an actor has received an attack order. */
	virtual void NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Event when an actor has received a begin construction order. */
	virtual void NotifyOnIssuedBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Event when an actor has received a continue construction order. */
	virtual void NotifyOnIssuedContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite);

	/** Event when an actor has received a gather order. */
	virtual void NotifyOnIssuedGatherOrder(APawn* OrderedPawn, AActor* ResourceSource);

	/** Event when an actor has received a move order. */
	virtual void NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a production order. */
	virtual void NotifyOnIssuedProductionOrder(AActor* OrderedActor, TSubclassOf<AActor> ProductClass);

	/** Event when an actor has received a stop order. */
	virtual void NotifyOnIssuedStopOrder(APawn* OrderedPawn);

	/** Event when the player has clicked a spot on the minimap. */
	virtual void NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition);

	/** Event when the a new subgroup of actors has been selected. */
	virtual void NotifyOnSelectedSubgroupChanged(TSubclassOf<AActor> Subgroup);

	/** Event when the set of selected actors of this player has changed. */
	virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when the team of this player has changed. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when vision info for this player has been replicated and is available. */
	virtual void NotifyOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo);

	/** Event when this player is now owning the specified actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Ownership", meta = (DisplayName = "OnActorOwnerChanged"))
	void ReceiveOnActorOwnerChanged(AActor* Actor);

	/** Event when the player begins placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementStarted"))
	void ReceiveOnBuildingPlacementStarted(TSubclassOf<AActor> BuildingClass);

	/** Event when the player confirms a location for placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementConfirmed"))
	void ReceiveOnBuildingPlacementConfirmed(TSubclassOf<AActor> BuildingClass, const FVector& Location);

	/** Event when the player receives an error placing a building at a specific location. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementError"))
	void ReceiveOnBuildingPlacementError(TSubclassOf<AActor> BuildingClass, const FVector& Location);

	/** Event when the player cancels placing a building. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Construction", meta = (DisplayName = "OnBuildingPlacementCancelled"))
	void ReceiveOnBuildingPlacementCancelled(TSubclassOf<AActor> BuildingClass);

	/** Event when an error has occurred that can be presented to the user. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Error", meta = (DisplayName = "OnErrorOccurred"))
	void ReceiveOnErrorOccurred(const FString& ErrorMessage);

	/** Event when the game has ended. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Game", meta = (DisplayName = "OnGameHasEnded"))
	void ReceiveOnGameHasEnded(bool bIsWinner);

	/** Event when a pawn has received an order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedOrder"))
	void ReceiveOnIssuedOrder(APawn* OrderedPawn, const FRTSOrderData& Order);

	/** Event when an actor has received an attack order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedAttackOrder"))
	void ReceiveOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Event when an actor has received a begin construction order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedBeginConstructionOrder"))
	void ReceiveOnIssuedBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation);

	/** Event when an actor has received a continue construction order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedContinueConstructionOrder"))
	void ReceiveOnIssuedContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite);

	/** Event when an actor has received a gather order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedGatherOrder"))
	void ReceiveOnIssuedGatherOrder(APawn* OrderedPawn, AActor* ResourceSource);

	/** Event when an actor has received a move order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedMoveOrder"))
	void ReceiveOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a production order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedProductionOrder"))
	void ReceiveOnIssuedProductionOrder(AActor* OrderedActor, TSubclassOf<AActor> ProductClass);

	/** Event when an actor has received a stop order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedStopOrder"))
	void ReceiveOnIssuedStopOrder(APawn* OrderedPawn);

	/** Event when the player has clicked a spot on the minimap. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Minimap", meta = (DisplayName = "OnMinimapClicked"))
	void ReceiveOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition);

	/** Event when the player state has been set up or replicated for this player. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnPlayerStateAvailable"))
	void ReceiveOnPlayerStateAvailable(ARTSPlayerState* NewPlayerState);

	/** Event when the a new subgroup of actors has been selected. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Selection", meta = (DisplayName = "OnSelectedSubgroupChanged"))
	void ReceiveOnSelectedSubgroupChanged(TSubclassOf<AActor> Subgroup);

	/** Event when the set of selected actors of this player has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Selection", meta = (DisplayName = "OnSelectionChanged"))
	void ReceiveOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when the team of this player has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when vision info for this player has been replicated and is available. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Vision", meta = (DisplayName = "OnVisionInfoAvailable"))
	void ReceiveOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo);


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Player state has been created or replicated and is now available. */
	virtual void OnPlayerStateAvailable(ARTSPlayerState* NewPlayerState);

private:
	/** Movement speed of the camera when moved with keys or mouse, in cm/sec. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Camera", meta = (ClampMin = 0))
	float CameraSpeed;

	/** How fast to zoom the camera in and out, in cm/sec. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Camera", meta = (ClampMin = 0))
	float CameraZoomSpeed;

	/** Minimum distance of the camera from the player pawn, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Camera", meta = (ClampMin = 0))
	float MinCameraDistance;

	/** Maximum distance of the camera from the player pawn, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Camera", meta = (ClampMin = 0))
	float MaxCameraDistance;

	/** Distance from the screen border at which the mouse cursor causes the camera to move, in pixels. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Camera", meta = (ClampMin = 0))
	int32 CameraScrollThreshold;

	/** Time between two group selections to be considered a double-selection (e.g. for centering the camera on that group). */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Input", meta = (ClampMin = 0))
	float DoubleGroupSelectionTime;

	/** Preview to use for placing buildings. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Construction")
	TSubclassOf<ARTSBuildingCursor> BuildingCursorClass;

	/** Orders that should be tried to apply for the input action IssueOrder, in order. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Orders")
	TArray<TSubclassOf<URTSOrder>> DefaultOrders;

	/** Actor classes which should be ignored when tracing for default order targets. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Orders")
	TArray<TSubclassOf<AActor>> DefaultOrderIgnoreTargetClasses;

	/** Provides bonuses for various gameplay elements for this player. */
	UPROPERTY(VisibleAnywhere, Category = "RTS")
	URTSPlayerAdvantageComponent* PlayerAdvantageComponent;

	/** Stores the resources available for this player. */
	UPROPERTY(VisibleAnywhere, Category = "RTS")
	URTSPlayerResourcesComponent* PlayerResourcesComponent;

	/** Volume that restricts the camera movement of this player. */
	UPROPERTY()
	ARTSCameraBoundsVolume* CameraBoundsVolume;

	/** Last horizontal axis input applied to camera movement. */
	float CameraLeftRightAxisValue;

	/** Last vertical axis input applied to camera movement. */
	float CameraUpDownAxisValue;

	/** Last zoom axis input applied to camera movement. */
	float CameraZoomAxisValue;

	/** Saved selections of this player. */
	UPROPERTY()
	TArray<FRTSControlGroup> ControlGroups;

	/** Actor currently hovered by this player. */
	UPROPERTY()
	AActor* HoveredActor;

	/** World position currently hovered by this player. */
	FVector HoveredWorldPosition;

	/** Actors selected by this player. */
	UPROPERTY()
	TArray<AActor*> SelectedActors;

	/** Type of actors whose subgroup is currently selected. */
	TSubclassOf<AActor> SelectedSubgroup;

	/** Last time a group was selected. */
	float LastSelectionTime;

	/** Saved camera locations of this player. */
	TArray<FVector> CameraLocations;

	/** Type of the building currently being placed, if any. */
	TSubclassOf<AActor> BuildingBeingPlacedClass;

	/** Current cursor for placing a new building. */
	UPROPERTY()
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

	/** Time to wait before playing the next selection sound, in seconds. */
	float SelectionSoundCooldownRemaining;


	/** Casts a ray from the current mouse position and collects the results. */
	bool GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults) const;

	/** Casts a box from the current selection frame and collects the results. */
	bool GetObjectsInSelectionFrame(TArray<FHitResult>& OutHitResults) const;

	/** Traces all relevant objects using the specified ray. */
	bool TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults) const;

	/** Checks whether the specified actor is valid and selectable. */
	bool IsSelectableActor(AActor* Actor) const;

	/** Automatically issues the most reasonable order for the current pointer position. */
	UFUNCTION()
	void IssueDefaultOrderToSelectedActors();

	/** Automatically issues the most reasonable order for the specified targets. */
	void IssueOrderTargetingObjectsToSelectedActors(TArray<FHitResult>& HitResults);

	/** Gets any member of the currently selected subgroup of selected actors. */
	bool GetSelectedSubgroupActorAndIndex(AActor** OutSelectedSubgroupActor, int32* OutSelectedSubgroupActorIndex);

	/** Selects the next or previous subgroup. */
	void SelectNextSubgroupInDirection(int32 Sign);

	/** Cancels constructing the specified actor, destroying the construction site. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCancelConstruction(AActor* ConstructionSite);

	/** Issues the specified order to the passed pawn. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueOrder(APawn* OrderedPawn, const FRTSOrderData& Order);

	/** Start producing the specified product at the specified actor. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartProduction(AActor* ProductionActor, TSubclassOf<AActor> ProductClass);

	/** Cancels the current production at the specified actor. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCancelProduction(AActor* ProductionActor);

	/** Surrenders the current match. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSurrender();

	/** Applies horizontal axis input to camera movement. */
	void MoveCameraLeftRight(float Value);

	/** Applies vertical axis input to camera movement. */
	void MoveCameraUpDown(float Value);

	/** Applies zoom input to camera movement. */
	void ZoomCamera(float Value);

	/** Gets the distance from the player camera to an object, on the ground, in cm. */
	float GetCameraDistance() const;

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

	/** Confirms placing the current building at the hovered location. */
	UFUNCTION()
	void ConfirmBuildingPlacement();

	/** Cancels placing the current building without effect. */
	UFUNCTION()
	void CancelBuildingPlacement();

	/** Cancels the construction of the first selected building. */
	UFUNCTION()
	void CancelConstruction();

	/** Cancels the current production of the first selected building. */
	UFUNCTION()
	void CancelProduction();
};
