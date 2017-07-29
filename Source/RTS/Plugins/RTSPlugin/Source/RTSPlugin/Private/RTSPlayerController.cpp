#include "RTSPluginPrivatePCH.h"
#include "RTSPlayerController.h"

#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"

#include "RTSAttackComponent.h"
#include "RTSAttackableComponent.h"
#include "RTSBuildingCursor.h"
#include "RTSCameraBoundsVolume.h"
#include "RTSCharacter.h"
#include "RTSCharacterAIController.h"
#include "RTSPlayerState.h"
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
	InputComponent->BindAction("AddSelection", IE_Pressed, this, &ARTSPlayerController::StartAddSelection);
	InputComponent->BindAction("AddSelection", IE_Released, this, &ARTSPlayerController::StopAddSelection);
	InputComponent->BindAction("ToggleSelection", IE_Pressed, this, &ARTSPlayerController::StartToggleSelection);
	InputComponent->BindAction("ToggleSelection", IE_Released, this, &ARTSPlayerController::StopToggleSelection);

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

	InputComponent->BindAction("ShowHealthBars", IE_Pressed, this, &ARTSPlayerController::StartShowingHealthBars);
	InputComponent->BindAction("ShowHealthBars", IE_Released, this, &ARTSPlayerController::StopShowingHealthBars);

	InputComponent->BindAction("ConfirmBuildingPlacement", IE_Released, this, &ARTSPlayerController::ConfirmBuildingPlacement);
	InputComponent->BindAction("CancelBuildingPlacement", IE_Released, this, &ARTSPlayerController::CancelBuildingPlacement);

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

	ARTSCharacter* Unit = Cast<ARTSCharacter>(Actor);

	if (Unit)
	{
		Unit->NotifyOnOwnerChanged(this);
	}
}

AActor* ARTSPlayerController::GetHoveredActor()
{
	return HoveredActor;
}

ARTSPlayerState* ARTSPlayerController::GetPlayerState()
{
	return Cast<ARTSPlayerState>(PlayerState);
}

TArray<AActor*> ARTSPlayerController::GetSelectedActors()
{
	return SelectedActors;
}

bool ARTSPlayerController::GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& HitResults)
{
	// Get ray.
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, HitResults);
}

bool ARTSPlayerController::GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& HitResults)
{
	// Get ray.
	FVector WorldOrigin = FVector(WorldPositionXY.X, WorldPositionXY.Y, HitResultTraceDistance / 2);
	FVector WorldDirection = -FVector::UpVector;

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, HitResults);
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

	return GetObjectsAtScreenPosition(MousePosition, HitResults);
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
		ARTSCharacter* Unit = *CharacterIt;

		FVector2D CharacterScreenPosition;

		if (UGameplayStatics::ProjectWorldToScreen(this, Unit->GetActorLocation(), CharacterScreenPosition))
		{
			if (SelectionFrame.Contains(FIntPoint(CharacterScreenPosition.X, CharacterScreenPosition.Y)))
			{
				FHitResult HitResult(Unit, nullptr, Unit->GetActorLocation(), FVector());
				HitResults.Add(HitResult);
			}
		}
	}

	return HitResults.Num() > 0;
}

bool ARTSPlayerController::TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& HitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);

	return World->LineTraceMultiByObjectType(
		HitResults,
		WorldOrigin,
		WorldOrigin + WorldDirection * HitResultTraceDistance,
		Params);
}

bool ARTSPlayerController::IsSelectableActor(AActor* Actor)
{
	// Check if valid.
	if (!IsValid(Actor))
	{
		return false;
	}

	// Check if selectable.
	auto SelectableComponent = Actor->FindComponentByClass<URTSSelectableComponent>();

	if (!SelectableComponent)
	{
		return false;
	}

	return true;
}

void ARTSPlayerController::IssueOrder()
{
    // Get objects at pointer position.
    TArray<FHitResult> HitResults;

    if (!GetObjectsAtPointerPosition(HitResults))
    {
        return;
    }

	IssueOrderTargetingObjects(HitResults);
}

void ARTSPlayerController::IssueOrderTargetingObjects(TArray<FHitResult>& HitResults)
{
	// Check if there's anybody to receive the order.
	if (SelectedActors.Num() == 0)
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
	ARTSTeamInfo* MyTeam = GetPlayerState()->Team;

	// Issue attack orders.
	for (auto SelectedActor : SelectedActors)
	{
		// Verify target.
		auto SelectedCharacter = Cast<ARTSCharacter>(SelectedActor);

		if (!SelectedCharacter)
		{
			continue;
		}

		auto TargetCharacter = Cast<ARTSCharacter>(Target);

		if (TargetCharacter && TargetCharacter->IsSameTeamAsCharacter(SelectedCharacter))
		{
			continue;
		}
		
		if (SelectedCharacter->FindComponentByClass<URTSAttackComponent>() == nullptr)
		{
			continue;
		}

		// Send attack order to server.
		ServerIssueAttackOrder(SelectedCharacter, Target);
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

void ARTSPlayerController::SelectActors(TArray<AActor*> Actors)
{
	// Clear selection.
	for (AActor* SelectedActor : SelectedActors)
	{
		ARTSCharacter* Unit = Cast<ARTSCharacter>(SelectedActor);

		if (Unit != nullptr)
		{
			Unit->NotifyOnDeselected();
		}
	}

	// Apply new selection.
	SelectedActors = Actors;

	for (AActor* SelectedActor : SelectedActors)
	{
		ARTSCharacter* Unit = Cast<ARTSCharacter>(SelectedActor);

		if (Unit != nullptr)
		{
			Unit->NotifyOnSelected();
		}
	}

	// Notify listeners.
	NotifyOnSelectionChanged(SelectedActors);
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

	SelectActors(ControlGroups[Index]);

	UE_LOG(RTSLog, Log, TEXT("Loaded selection from control group %d."), Index);
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

bool ARTSPlayerController::IsHealthBarHotkeyPressed()
{
	return bHealthBarHotkeyPressed;
}

void ARTSPlayerController::BeginBuildingPlacement(TSubclassOf<AActor> BuildingType, USkeletalMesh* PreviewMesh)
{
	// Spawn dummy building.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BuildingCursor = GetWorld()->SpawnActor<ARTSBuildingCursor>(BuildingCursorClass, SpawnParams);
	BuildingCursor->SetMesh(PreviewMesh);
	BuildingCursor->SetInvalidLocation();

	BuildingTypeBeingPlaced = BuildingType;
}

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
	TArray<AActor*> ActorsToSelect;

	if (bAddSelectionHotkeyPressed || bToggleSelectionHotkeyPressed)
	{
		ActorsToSelect = SelectedActors;
	}

    for (auto& HitResult : HitResults)
    {
		if (!IsSelectableActor(HitResult.Actor.Get()))
		{
			continue;
		}

		// Check how to apply selection.
		if (bToggleSelectionHotkeyPressed)
		{
			if (SelectedActors.Contains(HitResult.Actor))
			{
				// Deselect actor.
				ActorsToSelect.Remove(HitResult.Actor.Get());

				UE_LOG(RTSLog, Log, TEXT("Deselected actor %s."), *HitResult.Actor->GetName());
			}
			else if (!ActorsToSelect.Contains(HitResult.Actor))
			{
				// Select actor.
				ActorsToSelect.Add(HitResult.Actor.Get());

				UE_LOG(RTSLog, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
			}
		}
		else
		{
			if (ActorsToSelect.Contains(HitResult.Actor))
			{
				continue;
			}

			// Select actor.
			ActorsToSelect.Add(HitResult.Actor.Get());

			UE_LOG(RTSLog, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
		}
    }

	SelectActors(ActorsToSelect);

	bCreatingSelectionFrame = false;
}

void ARTSPlayerController::StartShowingHealthBars()
{
	bHealthBarHotkeyPressed = true;
}

void ARTSPlayerController::StopShowingHealthBars()
{
	bHealthBarHotkeyPressed = false;
}

void ARTSPlayerController::StartAddSelection()
{
	bAddSelectionHotkeyPressed = true;
}

void ARTSPlayerController::StopAddSelection()
{
	bAddSelectionHotkeyPressed = false;
}

void ARTSPlayerController::StartToggleSelection()
{
	bToggleSelectionHotkeyPressed = true;
}

void ARTSPlayerController::StopToggleSelection()
{
	bToggleSelectionHotkeyPressed = false;
}

void ARTSPlayerController::ConfirmBuildingPlacement()
{
	if (!BuildingCursor)
	{
		return;
	}

	// Remove dummy building.
	BuildingCursor->Destroy();
	BuildingCursor = nullptr;

	// Start construction.
	ServerConstructBuildingAtLocation(BuildingTypeBeingPlaced, HoveredWorldPosition);
}

void ARTSPlayerController::CancelBuildingPlacement()
{
	if (!BuildingCursor)
	{
		return;
	}

	// Remove dummy building.
	BuildingCursor->Destroy();
	BuildingCursor = nullptr;
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

void ARTSPlayerController::NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition)
{
	APawn* PlayerPawn = GetPawn();

	if (!PlayerPawn)
	{
		return;
	}

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// Move camera.
		FVector NewCameraLocation = FVector(WorldPosition.X, WorldPosition.Y, PlayerPawn->GetActorLocation().Z);
		PlayerPawn->SetActorLocation(NewCameraLocation);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		// Get objects at pointer position.
		TArray<FHitResult> HitResults;

		if (!GetObjectsAtWorldPosition(WorldPosition, HitResults))
		{
			return;
		}

		IssueOrderTargetingObjects(HitResults);
	}
	
	// Notify listeners.
	ReceiveOnMinimapClicked(InMouseEvent, MinimapPosition, WorldPosition);
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

	// Get hovered actors.
	HoveredActor = nullptr;

	TArray<FHitResult> HitResults;

	if (GetObjectsAtPointerPosition(HitResults))
	{
		for (auto& HitResult : HitResults)
		{
			// Check if hit any actor.
			if (HitResult.Actor == nullptr)
			{
				// Store hovered world position.
				HoveredWorldPosition = HitResult.Location;

				// Update position of building being placed.
				if (BuildingCursor)
				{
					BuildingCursor->SetActorLocation(HoveredWorldPosition);
				}
				continue;
			}

			// Check if hit selectable actor.
			auto SelectableComponent = HitResult.Actor->FindComponentByClass<URTSSelectableComponent>();

			if (!SelectableComponent)
			{
				continue;
			}

			// Set hovered actor.
			HoveredActor = HitResult.Actor.Get();
		}
	}
}

void ARTSPlayerController::ServerConstructBuildingAtLocation_Implementation(TSubclassOf<AActor> BuildingType, FVector Location)
{
	// Spawn building to construct.
	// TODO: Add different styles of building construction (e.g. C&C, Warcraft).
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(BuildingType, Location, FRotator::ZeroRotator, SpawnParams);

	UE_LOG(RTSLog, Log, TEXT("Constructed building %s at %s."), *BuildingType.Get()->GetName(), *Location.ToString());

	// TODO: Raise event.
}

bool ARTSPlayerController::ServerConstructBuildingAtLocation_Validate(TSubclassOf<AActor> BuildingType, FVector Location)
{
	return true;
}