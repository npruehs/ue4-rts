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
	
protected:
    virtual void BeginPlay() override;
	
private:
    UFUNCTION()
    void OnLeftMouseButtonReleased();
};
