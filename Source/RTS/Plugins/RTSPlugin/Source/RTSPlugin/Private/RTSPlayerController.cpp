#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerController.h"

#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "RTSAttackComponent.h"
#include "RTSAttackableComponent.h"
#include "RTSCameraBoundsVolume.h"
#include "RTSCharacter.h"
#include "RTSCharacterAIController.h"
#include "RTSSelectableComponent.h"


void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();  
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enable mouse input.
	APlayerController::bShowMouseCursor = true;
	APlayerController::bEnableClickEvents = true;
	APlayerController::bEnableMouseOverEvents = true;

	// Bind actions.
	InputComponent->BindAction("Select", IE_Pressed, this, &ARTSPlayerController::StartSelectActors);
	InputComponent->BindAction("Select", IE_Released, this, &ARTSPlayerController::FinishSelectActors);

	InputComponent->BindAction("IssueOrder", IE_Released, this, &ARTSPlayerController::IssueOrder);
	InputComponent->BindAction("IssueStopOrder", IE_Released, this, &ARTSPlayerController::IssueStopOrder);

	InputComponent->BindAction("SaveControlGroup0", IE_Released, this, &ARTSPlayerController::SaveControlGroup0);
	InputComponent->BindAction("SaveControlGroup1", IE_Released, this, &ARTSPlayerController::SaveControlGroup1);
	InputComponent->BindAction("SaveControlGroup2", IE_Released, this, &ARTSPlayerController::SaveControlGroup2);
	InputComponent->BindAction("SaveControlGroup3", IE_Released, this, &ARTSPlayerController::SaveControlGroup3);
	InputComponent->BindAction("SaveControlGroup4", IE_Released, this, &ARTSPlayerController::SaveControlGroup4);
	InputComponent->BindAction("SaveControlGroup5", IE_Released, this, &ARTSPlayerController::SaveControlGroup5);
	InputComponent->BindAction("SaveControlGroup6", IE_Released, this, &ARTSPlayerController::SaveControlGroup6);
	InputComponent->BindAction("SaveControlGroup7", IE_Released, this, &ARTSPlayerController::SaveControlGroup7);
	InputComponent->BindAction("SaveControlGroup8", IE_Released, this, &ARTSPlayerController::SaveControlGroup8);
	InputComponent->BindAction("SaveControlGroup9", IE_Released, this, &ARTSPlayerController::SaveControlGroup9);

	InputComponent->BindAction("LoadControlGroup0", IE_Released, this, &ARTSPlayerController::LoadControlGroup0);
	InputComponent->BindAction("LoadControlGroup1", IE_Released, this, &ARTSPlayerController::LoadControlGroup1);
	InputComponent->BindAction("LoadControlGroup2", IE_Released, this, &ARTSPlayerController::LoadControlGroup2);
	InputComponent->BindAction("LoadControlGroup3", IE_Released, this, &ARTSPlayerController::LoadControlGroup3);
	InputComponent->BindAction("LoadControlGroup4", IE_Released, this, &ARTSPlayerController::LoadControlGroup4);
	InputComponent->BindAction("LoadControlGroup5", IE_Released, this, &ARTSPlayerController::LoadControlGroup5);
	InputComponent->BindAction("LoadControlGroup6", IE_Released, this, &ARTSPlayerController::LoadControlGroup6);
	InputComponent->BindAction("LoadControlGroup7", IE_Released, this, &ARTSPlayerController::LoadControlGroup7);
	InputComponent->BindAction("LoadControlGroup8", IE_Released, this, &ARTSPlayerController::LoadControlGroup8);
	InputComponent->BindAction("LoadControlGroup9", IE_Released, this, &ARTSPlayerController::LoadControlGroup9);

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

	// Setup control groups.
	ControlGroups.SetNum(10);
}

void ARTSPlayerController::TransferOwnership(AActor* Actor)
{
	// Set owning player.
	Actor->SetOwner(this);

	UE_LOG(RTSLog, Log, TEXT("Player %s is now owning %s."), *GetName(), *Actor->GetName());

	// Notify listeners.
	NotifyOnActorOwnerChanged(Actor);

	ARTSCharacter* Character = Cast<ARTSCharacter>(Actor);

	if (Character)
	{
		Character->NotifyOnOwnerChanged(this);
	}
}

bool ARTSPlayerController::GetSelectionFrame(FIntRect& OutSelectionFrame)
{
	if (!bCreatingSelectionFrame)
	{
		return false;
	}

	// Get mouse input.
	float MouseX;
	float MouseY;

	if (!GetMousePosition(MouseX, MouseY))
	{
		return false;
	}

	OutSelectionFrame = FIntRect(
		FIntPoint(SelectionFrameMouseStartPosition.X, SelectionFrameMouseStartPosition.Y),
		FIntPoint(MouseX, MouseY));

	return true;
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

bool ARTSPlayerController::GetObjectsInSelectionFrame(TArray<FHitResult>& HitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	// Get selection frame.
	FIntRect SelectionFrame;
	
	if (!GetSelectionFrame(SelectionFrame))
	{
		return false;
	}

	if (SelectionFrame.Area() < 10)
	{
		// Selection frame too small - just consider left-click.
		return GetObjectsAtPointerPosition(HitResults);
	}

	// Iterate all characters.
	HitResults.Reset();

	for (TActorIterator<ARTSCharacter> CharacterIt(GetWorld()); CharacterIt; ++CharacterIt)
	{
		ARTSCharacter* Character = *CharacterIt;

		FVector2D CharacterScreenPosition;

		if (UGameplayStatics::ProjectWorldToScreen(this, Character->GetActorLocation(), CharacterScreenPosition))
		{
			if (SelectionFrame.Contains(FIntPoint(CharacterScreenPosition.X, CharacterScreenPosition.Y)))
			{
				FHitResult HitResult(Character, nullptr, Character->GetActorLocation(), FVector());
				HitResults.Add(HitResult);
			}
		}
	}

	return HitResults.Num() > 0;
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
			// Check if hit attackable actor.
			auto AttackableComponent = HitResult.Actor->FindComponentByClass<URTSAttackableComponent>();

			if (!AttackableComponent)
			{
				continue;
			}
			else
			{
				// Issue attack order.
				IssueAttackOrder(HitResult.Actor.Get());
				return;
			}
        }

        // Issue move order.
        IssueMoveOrder(HitResult.Location);
        return;
    }
}

void ARTSPlayerController::IssueAttackOrder(AActor* Target)
{
	// Issue attack orders.
	for (auto SelectedActor : SelectedActors)
	{
		// Verify pawn and owner.
		auto SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		if (SelectedPawn->FindComponentByClass<URTSAttackComponent>() == nullptr)
		{
			continue;
		}

		// Send attack order to server.
		ServerIssueAttackOrder(SelectedPawn, Target);
		UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to attack %s."), *SelectedActor->GetName(), *Target->GetName());

		// Notify listeners.
		NotifyOnIssuedAttackOrder(SelectedActor, Target);
	}
}

void ARTSPlayerController::ServerIssueAttackOrder_Implementation(APawn* OrderedPawn, AActor* Target)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue attack order.
	PawnController->IssueAttackOrder(Target);
	UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to attack %s."), *OrderedPawn->GetName(), *Target->GetName());

	// Notify listeners.
	NotifyOnIssuedAttackOrder(OrderedPawn, Target);
}

bool ARTSPlayerController::ServerIssueAttackOrder_Validate(APawn* OrderedPawn, AActor* Target)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::IssueMoveOrder(const FVector& TargetLocation)
{
    // Issue move orders.
    for (auto SelectedActor : SelectedActors)
    {
        // Verify pawn and owner.
        auto SelectedPawn = Cast<APawn>(SelectedActor);

        if (!SelectedPawn)
        {
            continue;
        }

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Send move order to server.
		ServerIssueMoveOrder(SelectedPawn, TargetLocation);
        UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to move to %s."), *SelectedActor->GetName(), *TargetLocation.ToString());

        // Notify listeners.
        NotifyOnIssuedMoveOrder(SelectedActor, TargetLocation);
    }
}

void ARTSPlayerController::IssueStopOrder()
{
	// Issue stop orders.
	for (auto SelectedActor : SelectedActors)
	{
		// Verify pawn and owner.
		auto SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Send stop order to server.
		ServerIssueStopOrder(SelectedPawn);
		UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to stop."), *SelectedActor->GetName());

		// Notify listeners.
		NotifyOnIssuedStopOrder(SelectedActor);
	}
}

void ARTSPlayerController::SaveControlGroup(int Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	// Save control group.
	ControlGroups[Index] = SelectedActors;

	UE_LOG(RTSLog, Log, TEXT("Saved selection to control group %d."), Index);
}

void ARTSPlayerController::SaveControlGroup0() { SaveControlGroup(0); }
void ARTSPlayerController::SaveControlGroup1() { SaveControlGroup(1); }
void ARTSPlayerController::SaveControlGroup2() { SaveControlGroup(2); }
void ARTSPlayerController::SaveControlGroup3() { SaveControlGroup(3); }
void ARTSPlayerController::SaveControlGroup4() { SaveControlGroup(4); }
void ARTSPlayerController::SaveControlGroup5() { SaveControlGroup(5); }
void ARTSPlayerController::SaveControlGroup6() { SaveControlGroup(6); }
void ARTSPlayerController::SaveControlGroup7() { SaveControlGroup(7); }
void ARTSPlayerController::SaveControlGroup8() { SaveControlGroup(8); }
void ARTSPlayerController::SaveControlGroup9() { SaveControlGroup(9); }

void ARTSPlayerController::LoadControlGroup(int Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	SelectedActors = ControlGroups[Index];

	UE_LOG(RTSLog, Log, TEXT("Loaded selection from control group %d."), Index);

	// Notify listeners.
	NotifyOnSelectionChanged(SelectedActors);
}

void ARTSPlayerController::LoadControlGroup0() { LoadControlGroup(0); }
void ARTSPlayerController::LoadControlGroup1() { LoadControlGroup(1); }
void ARTSPlayerController::LoadControlGroup2() { LoadControlGroup(2); }
void ARTSPlayerController::LoadControlGroup3() { LoadControlGroup(3); }
void ARTSPlayerController::LoadControlGroup4() { LoadControlGroup(4); }
void ARTSPlayerController::LoadControlGroup5() { LoadControlGroup(5); }
void ARTSPlayerController::LoadControlGroup6() { LoadControlGroup(6); }
void ARTSPlayerController::LoadControlGroup7() { LoadControlGroup(7); }
void ARTSPlayerController::LoadControlGroup8() { LoadControlGroup(8); }
void ARTSPlayerController::LoadControlGroup9() { LoadControlGroup(9); }

void ARTSPlayerController::ServerIssueMoveOrder_Implementation(APawn* OrderedPawn, const FVector& TargetLocation)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue move order.
	PawnController->IssueMoveOrder(TargetLocation);
	UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to move to %s."), *OrderedPawn->GetName(), *TargetLocation.ToString());

	// Notify listeners.
	NotifyOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

bool ARTSPlayerController::ServerIssueMoveOrder_Validate(APawn* OrderedPawn, const FVector& TargetLocation)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::ServerIssueStopOrder_Implementation(APawn* OrderedPawn)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue stop order.
	PawnController->IssueStopOrder();
	UE_LOG(RTSLog, Log, TEXT("Ordered actor %s to stop."), *OrderedPawn->GetName());

	// Notify listeners.
	NotifyOnIssuedStopOrder(OrderedPawn);
}

bool ARTSPlayerController::ServerIssueStopOrder_Validate(APawn* OrderedPawn)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::StartSelectActors()
{
	// Get mouse input.
	float MouseX;
	float MouseY;

	if (GetMousePosition(MouseX, MouseY))
	{
		SelectionFrameMouseStartPosition = FVector2D(MouseX, MouseY);
		bCreatingSelectionFrame = true;
	}
}

void ARTSPlayerController::FinishSelectActors()
{
    // Get objects at pointer position.
    TArray<FHitResult> HitResults;
    
    if (!GetObjectsInSelectionFrame(HitResults))
    {
		bCreatingSelectionFrame = false;
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

		if (SelectedActors.Contains(HitResult.Actor))
		{
			continue;
		}

        // Check if hit selectable actor.
        auto SelectableComponent = HitResult.Actor->FindComponentByClass<URTSSelectableComponent>();

        if (!SelectableComponent)
        {
            continue;
        }

        // Select actor.
        SelectedActors.Add(HitResult.Actor.Get());

        UE_LOG(RTSLog, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
    }

    // Notify listeners.
    NotifyOnSelectionChanged(SelectedActors);

	bCreatingSelectionFrame = false;
}

void ARTSPlayerController::MoveCameraLeftRight(float Value)
{
    CameraLeftRightAxisValue = Value;
}

void ARTSPlayerController::MoveCameraUpDown(float Value)
{
    CameraUpDownAxisValue = Value;
}

void ARTSPlayerController::NotifyOnActorOwnerChanged(AActor* Actor)
{
	ReceiveOnActorOwnerChanged(Actor);
}

void ARTSPlayerController::NotifyOnIssuedAttackOrder(AActor* Actor, AActor* Target)
{
	ReceiveOnIssuedAttackOrder(Actor, Target);
}

void ARTSPlayerController::NotifyOnIssuedMoveOrder(AActor* Actor, const FVector& TargetLocation)
{
    ReceiveOnIssuedMoveOrder(Actor, TargetLocation);
}

void ARTSPlayerController::NotifyOnIssuedStopOrder(AActor* Actor)
{
	ReceiveOnIssuedStopOrder(Actor);
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
