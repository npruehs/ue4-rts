#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/PlayerController.h"

#include "RTSPlayerController.generated.h"

class ARTSCameraBoundsVolume;


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
    int CameraScrollThreshold;


	virtual void PlayerTick(float DeltaTime) override;


	/** Orders all selected units to attack the specified unit. */
	UFUNCTION(BlueprintCallable)
	void IssueAttackOrder(AActor* Target);

	/** Orders all selected units to move to the specified location. */
	UFUNCTION(BlueprintCallable)
	void IssueMoveOrder(const FVector& TargetLocation);

	/** Orders all selected units to stop all current actions. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();


	/** Event when this player is now owning the specified actor. */
	virtual void NotifyOnActorOwnerChanged(AActor* Actor);

	/** Event when an actor has received an attack order. */
	virtual void NotifyOnIssuedAttackOrder(AActor* Actor, AActor* Target);

    /** Event when an actor has received a move order. */
    virtual void NotifyOnIssuedMoveOrder(AActor* Actor, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	virtual void NotifyOnIssuedStopOrder(AActor* Actor);

    /** Event when the set of selected actors of this player has changed. */
    virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when this player is now owning the specified actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Ownership", meta = (DisplayName = "OnActorOwnerChanged"))
	void ReceiveOnActorOwnerChanged(AActor* Actor);

	/** Event when an actor has received an attack order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedAttackOrder"))
	void ReceiveOnIssuedAttackOrder(AActor* Actor, AActor* Target);

    /** Event when an actor has received a move order. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedMoveOrder"))
    void ReceiveOnIssuedMoveOrder(AActor* Actor, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedStopOrder"))
	void ReceiveOnIssuedStopOrder(AActor* Actor);

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

    /** Actors selected by this player. */
    TArray<AActor*> SelectedActors;


    /** Casts a ray from the current pointer position and collects the results. */
    bool GetObjectsAtPointerPosition(TArray<FHitResult>& HitResults);

    /** Automatically issues the most reasonable order for the current pointer position. */
    UFUNCTION()
    void IssueOrder();

	/** Orders the passed unit to attack the specified unit. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Orders the passed unit to move to the specified location. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Orders the passed unit to stop all current actions. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueStopOrder(APawn* OrderedPawn);

    /** Applies horizontal axis input to camera movement. */
    void MoveCameraLeftRight(float Value);

    /** Applies vertical axis input to camera movement. */
    void MoveCameraUpDown(float Value);

    /** Selects the first selectable actor at the current pointer position. */
    UFUNCTION()
    void SelectActor();
};
