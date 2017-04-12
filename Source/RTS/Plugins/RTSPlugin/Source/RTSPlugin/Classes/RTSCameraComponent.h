// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Camera/CameraComponent.h"
#include "RTSCameraComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URTSCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
    // Sets default values for this component's properties
    URTSCameraComponent();

    UPROPERTY(EditAnywhere, Category = "RTS")
    float CameraDistance;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
