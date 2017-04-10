// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "RTSCameraComponent.h"

#include "GameFramework/Actor.h"



// Sets default values for this component's properties
URTSCameraComponent::URTSCameraComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void URTSCameraComponent::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void URTSCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Apply camera distance.
    AActor* Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    FVector Location = Owner->GetActorLocation();
    Location.Z = CameraDistance;
    Owner->SetActorLocation(Location);
}