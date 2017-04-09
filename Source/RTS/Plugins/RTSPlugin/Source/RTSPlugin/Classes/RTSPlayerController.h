// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"


UCLASS()
class ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    /** Event when the set of selected actors of this player has changed. */
    virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

    /** Event when the set of selected actors of this player has changed. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSelectionChanged"), Category = "RTS")
    void ReceiveOnSelectionChanged(const TArray<AActor*>& Selection);


protected:
    virtual void BeginPlay() override;
	
private:
    TArray<AActor*> SelectedActors;

    UFUNCTION()
    void OnLeftMouseButtonReleased();
};
