#pragma once

#include "RTSPluginPrivatePCH.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CameraBlockingVolume.h"
#include "RTSPlayerController.generated.h"

class ARTSCameraBoundsVolume;

UCLASS()
class ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "RTS Camera")
    float CameraSpeed;

    UPROPERTY(EditAnywhere, Category = "RTS Camera")
    float CameraScrollThreshold;


    /** Event when the set of selected actors of this player has changed. */
    virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

    /** Event when the set of selected actors of this player has changed. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSelectionChanged"), Category = "RTS")
    void ReceiveOnSelectionChanged(const TArray<AActor*>& Selection);

    virtual void PlayerTick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
	
private:
    TArray<AActor*> SelectedActors;

    float CameraLeftRightAxisValue;
    float CameraUpDownAxisValue;

    ARTSCameraBoundsVolume* CameraBoundsVolume;

    UFUNCTION()
    void OnLeftMouseButtonReleased();

    void OnMoveCameraLeftRight(float Value);
    void OnMoveCameraUpDown(float Value);
};
