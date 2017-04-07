// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "RTSPlayerController.h"

#include "Components/InputComponent.h"

#include "Engine/Engine.h"

void ARTSPlayerController::BeginPlay()
{
    APlayerController::BeginPlay();

    // Enable mouse input.
    APlayerController::bShowMouseCursor = true;
    APlayerController::bEnableClickEvents = true;
    APlayerController::bEnableMouseOverEvents = true;

    // Bind actions.
    InputComponent->BindAction("Select", IE_Released, this, &ARTSPlayerController::OnLeftMouseButtonReleased);
}

void ARTSPlayerController::OnLeftMouseButtonReleased()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
}