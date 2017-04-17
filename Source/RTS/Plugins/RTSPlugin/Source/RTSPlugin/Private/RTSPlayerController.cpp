#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerController.h"

#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "RTSCameraBoundsVolume.h"
#include "RTSCharacterAIController.h"
#include "RTSSelectableComponent.h"


void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enable mouse input.
    APlayerController::bShowMouseCursor = true;
    APlayerController::bEnableClickEvents = true;
    APlayerController::bEnableMouseOverEvents = true;

    // Bind actions.
    InputComponent->BindAction("Select", IE_Released, this, &ARTSPlayerController::SelectActor);
    InputComponent->BindAction("IssueOrder", IE_Released, this, &ARTSPlayerController::IssueOrder);
    InputComponent->BindAxis("MoveCameraLeftRight", this, &ARTSPlayerController::MoveCameraLeftRight);
    InputComponent->BindAxis("MoveCameraUpDown", this, &ARTSPlayerController::MoveCameraUpDown);

    // Get camera bounds.
    for (TActorIterator<ARTSCameraBoundsVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        CameraBoundsVolume = *ActorItr;
        break;
    }

    if (!CameraBoundsVolume)
    {
        UE_LOG(RTSLog, Warning, TEXT("No RTSCameraBoundsVolume found. Camera will be able to move anywhere."));
    }
}

bool ARTSPlayerController::GetObjectsAtPointerPosition(TArray<FHitResult>& HitResults)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return false;
    }

    // Get local player viewport.
    ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

    if (!LocalPlayer || !LocalPlayer->ViewportClient)
    {
        return false;
    }

    // Get mouse position.
    FVector2D MousePosition;
    if (!LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
    {
        return false;
    }

    // Get ray.
    FVector WorldOrigin;
    FVector WorldDirection;
    if (!UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection))
    {
        return false;
    }

    // Cast ray.
    FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);

    return World->LineTraceMultiByObjectType(
        HitResults,
        WorldOrigin,
        WorldOrigin + WorldDirection * HitResultTraceDistance,
        Params);
}

void ARTSPlayerController::IssueOrder()
{
    // Check if there's anybody to receive the order.
    if (SelectedActors.Num() == 0)
    {
        return;
    }

    // Get objects at pointer position.
    TArray<FHitResult> HitResults;

    if (!GetObjectsAtPointerPosition(HitResults))
    {
        return;
    }

    // Get target location.
    for (auto& HitResult : HitResults)
    {
        if (HitResult.Actor != nullptr)
        {
            continue;
        }

        // Issue move order.
        IssueMoveOrder(HitResult.Location);
        return;
    }
}

void ARTSPlayerController::IssueMoveOrder(const FVector& TargetLocation)
{
    // Issue move orders.
    for (auto SelectedActor : SelectedActors)
    {
        // Get RTS AI Controller.
        auto SelectedPawn = Cast<APawn>(SelectedActor);

        if (!SelectedPawn)
        {
            continue;
        }

        auto PawnController = Cast<ARTSCharacterAIController>(SelectedPawn->GetController());

        if (!PawnController)
        {
            continue;
        }

        // Issue move order.
        PawnController->IssueMoveOrder(TargetLocation);
        UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to move to %s."), *SelectedActor->GetName(), *TargetLocation.ToString());

        // Notify listeners.
        NotifyOnIssuedMoveOrder(SelectedActor, TargetLocation);
    }
}

void ARTSPlayerController::SelectActor()
{
    // Get objects at pointer position.
    TArray<FHitResult> HitResults;
    
    if (!GetObjectsAtPointerPosition(HitResults))
    {
        return;
    }

    // Check results.
    SelectedActors.Empty();

    for (auto& HitResult : HitResults)
    {
        // Check if hit any actor.
        if (HitResult.Actor == nullptr)
        {
            continue;
        }

        // Check if hit selectable actor.
        auto SelectableComponent = HitResult.Actor->FindComponentByClass<URTSSelectableComponent>();

        if (!SelectableComponent)
        {
            continue;
        }

        // Select single actor.
        SelectedActors.Add(HitResult.Actor.Get());

        UE_LOG(RTSLog, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
        break;
    }

    // Notify listeners.
    NotifyOnSelectionChanged(SelectedActors);
}

void ARTSPlayerController::MoveCameraLeftRight(float Value)
{
    CameraLeftRightAxisValue = Value;
}

void ARTSPlayerController::MoveCameraUpDown(float Value)
{
    CameraUpDownAxisValue = Value;
}

void ARTSPlayerController::NotifyOnIssuedMoveOrder(AActor* Actor, const FVector& TargetLocation)
{
    ReceiveOnIssuedMoveOrder(Actor, TargetLocation);
}

void ARTSPlayerController::NotifyOnSelectionChanged(const TArray<AActor*>& Selection)
{
    ReceiveOnSelectionChanged(Selection);
}

void ARTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    APawn* PlayerPawn = GetPawn();

    if (!PlayerPawn)
    {
        return;
    }

    // Get mouse input.
    float MouseX;
    float MouseY;

    const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    const float ScrollBorderRight = ViewportSize.X - CameraScrollThreshold;
    const float ScrollBorderTop = ViewportSize.Y - CameraScrollThreshold;

    if (GetMousePosition(MouseX, MouseY))
    {
        if (MouseX <= CameraScrollThreshold)
        {
            CameraLeftRightAxisValue -= 1 - (MouseX / CameraScrollThreshold);
        }
        else if (MouseX >= ScrollBorderRight)
        {
            CameraLeftRightAxisValue += (MouseX - ScrollBorderRight) / CameraScrollThreshold;
        }

        if (MouseY <= CameraScrollThreshold)
        {
            CameraUpDownAxisValue += 1 - (MouseY / CameraScrollThreshold);
        }
        else if (MouseY >= ScrollBorderTop)
        {
            CameraUpDownAxisValue -= (MouseY - ScrollBorderTop) / CameraScrollThreshold;
        }
    }

    // Apply input.
    CameraLeftRightAxisValue = FMath::Clamp(CameraLeftRightAxisValue, -1.0f, +1.0f);
    CameraUpDownAxisValue = FMath::Clamp(CameraUpDownAxisValue, -1.0f, +1.0f);
    
    FVector Location = PlayerPawn->GetActorLocation();
    Location += FVector::RightVector * CameraSpeed * CameraLeftRightAxisValue * DeltaTime;
    Location += FVector::ForwardVector * CameraSpeed * CameraUpDownAxisValue * DeltaTime;

    // Enforce camera bounds.
    if (!CameraBoundsVolume || CameraBoundsVolume->EncompassesPoint(Location))
    {
        PlayerPawn->SetActorLocation(Location);
    }
}
