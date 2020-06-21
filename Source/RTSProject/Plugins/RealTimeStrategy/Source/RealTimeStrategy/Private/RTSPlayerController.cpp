#include "RTSPlayerController.h"

#include "EngineUtils.h"
#include "Landscape.h"
#include "Camera/CameraComponent.h"
#include "Components/BrushComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "RTSCameraBoundsVolume.h"
#include "RTSPawnAIController.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSLog.h"
#include "RTSNameComponent.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerAdvantageComponent.h"
#include "RTSPlayerState.h"
#include "RTSSelectableComponent.h"
#include "RTSTeamInfo.h"
#include "Combat/RTSAttackComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "Construction/RTSBuildingCursor.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "Economy/RTSPlayerResourcesComponent.h"
#include "Economy/RTSResourceSourceComponent.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSConstructionLibrary.h"
#include "Libraries/RTSGameplayLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Libraries/RTSOrderLibrary.h"
#include "Orders/RTSAttackOrder.h"
#include "Orders/RTSBeginConstructionOrder.h"
#include "Orders/RTSContinueConstructionOrder.h"
#include "Orders/RTSGatherOrder.h"
#include "Orders/RTSMoveOrder.h"
#include "Orders/RTSStopOrder.h"
#include "Production/RTSProductionComponent.h"
#include "Production/RTSProductionCostComponent.h"
#include "Vision/RTSFogOfWarActor.h"
#include "Vision/RTSVisionInfo.h"
#include "Vision/RTSVisionManager.h"


ARTSPlayerController::ARTSPlayerController(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    PlayerAdvantageComponent = CreateDefaultSubobject<URTSPlayerAdvantageComponent>(TEXT("Player Advantage"));
    PlayerResourcesComponent = CreateDefaultSubobject<URTSPlayerResourcesComponent>(TEXT("Player Resources"));

  	// Set reasonable default values.
    CameraSpeed = 1000.0f;
    CameraZoomSpeed = 4000.0f;

    MinCameraDistance = 500.0f;
	MaxCameraDistance = 2500.0f;

	CameraScrollThreshold = 20.0f;

    DoubleGroupSelectionTime = 0.2f;
}

void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Allow immediate updates for interested listeners.
    for (int32 Index = 0; Index < PlayerResourcesComponent->GetResourceTypes().Num(); ++Index)
    {
        PlayerResourcesComponent->OnResourcesChanged.Broadcast(
            PlayerResourcesComponent->GetResourceTypes()[Index],
            0.0f,
            PlayerResourcesComponent->GetResources(PlayerResourcesComponent->GetResourceTypes()[Index]),
            true);
    }
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enable mouse input.
	APlayerController::bShowMouseCursor = true;
	APlayerController::bEnableClickEvents = true;
	APlayerController::bEnableMouseOverEvents = true;

	// Bind actions.
	InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &ARTSPlayerController::StartSelectActors);
	InputComponent->BindAction(TEXT("Select"), IE_Released, this, &ARTSPlayerController::FinishSelectActors);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Pressed, this, &ARTSPlayerController::StartAddSelection);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Released, this, &ARTSPlayerController::StopAddSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Pressed, this, &ARTSPlayerController::StartToggleSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Released, this, &ARTSPlayerController::StopToggleSelection);

    InputComponent->BindAction(TEXT("SelectNextSubgroup"), IE_Pressed, this, &ARTSPlayerController::SelectNextSubgroup);
    InputComponent->BindAction(TEXT("SelectPreviousSubgroup"), IE_Pressed, this, &ARTSPlayerController::SelectPreviousSubgroup);

	InputComponent->BindAction(TEXT("IssueOrder"), IE_Released, this, &ARTSPlayerController::IssueDefaultOrder);
	InputComponent->BindAction(TEXT("IssueStopOrder"), IE_Released, this, &ARTSPlayerController::IssueStopOrder);

	InputComponent->BindAction(TEXT("SaveControlGroup0"), IE_Released, this, &ARTSPlayerController::SaveControlGroup0);
	InputComponent->BindAction(TEXT("SaveControlGroup1"), IE_Released, this, &ARTSPlayerController::SaveControlGroup1);
	InputComponent->BindAction(TEXT("SaveControlGroup2"), IE_Released, this, &ARTSPlayerController::SaveControlGroup2);
	InputComponent->BindAction(TEXT("SaveControlGroup3"), IE_Released, this, &ARTSPlayerController::SaveControlGroup3);
	InputComponent->BindAction(TEXT("SaveControlGroup4"), IE_Released, this, &ARTSPlayerController::SaveControlGroup4);
	InputComponent->BindAction(TEXT("SaveControlGroup5"), IE_Released, this, &ARTSPlayerController::SaveControlGroup5);
	InputComponent->BindAction(TEXT("SaveControlGroup6"), IE_Released, this, &ARTSPlayerController::SaveControlGroup6);
	InputComponent->BindAction(TEXT("SaveControlGroup7"), IE_Released, this, &ARTSPlayerController::SaveControlGroup7);
	InputComponent->BindAction(TEXT("SaveControlGroup8"), IE_Released, this, &ARTSPlayerController::SaveControlGroup8);
	InputComponent->BindAction(TEXT("SaveControlGroup9"), IE_Released, this, &ARTSPlayerController::SaveControlGroup9);

	InputComponent->BindAction(TEXT("LoadControlGroup0"), IE_Released, this, &ARTSPlayerController::LoadControlGroup0);
	InputComponent->BindAction(TEXT("LoadControlGroup1"), IE_Released, this, &ARTSPlayerController::LoadControlGroup1);
	InputComponent->BindAction(TEXT("LoadControlGroup2"), IE_Released, this, &ARTSPlayerController::LoadControlGroup2);
	InputComponent->BindAction(TEXT("LoadControlGroup3"), IE_Released, this, &ARTSPlayerController::LoadControlGroup3);
	InputComponent->BindAction(TEXT("LoadControlGroup4"), IE_Released, this, &ARTSPlayerController::LoadControlGroup4);
	InputComponent->BindAction(TEXT("LoadControlGroup5"), IE_Released, this, &ARTSPlayerController::LoadControlGroup5);
	InputComponent->BindAction(TEXT("LoadControlGroup6"), IE_Released, this, &ARTSPlayerController::LoadControlGroup6);
	InputComponent->BindAction(TEXT("LoadControlGroup7"), IE_Released, this, &ARTSPlayerController::LoadControlGroup7);
	InputComponent->BindAction(TEXT("LoadControlGroup8"), IE_Released, this, &ARTSPlayerController::LoadControlGroup8);
	InputComponent->BindAction(TEXT("LoadControlGroup9"), IE_Released, this, &ARTSPlayerController::LoadControlGroup9);

	InputComponent->BindAxis(TEXT("MoveCameraLeftRight"), this, &ARTSPlayerController::MoveCameraLeftRight);
	InputComponent->BindAxis(TEXT("MoveCameraUpDown"), this, &ARTSPlayerController::MoveCameraUpDown);
    InputComponent->BindAxis(TEXT("ZoomCamera"), this, &ARTSPlayerController::ZoomCamera);

    InputComponent->BindAction(TEXT("SaveCameraLocation0"), IE_Pressed, this, &ARTSPlayerController::SaveCameraLocationWithIndex<0>);
    InputComponent->BindAction(TEXT("SaveCameraLocation1"), IE_Pressed, this, &ARTSPlayerController::SaveCameraLocationWithIndex<1>);
    InputComponent->BindAction(TEXT("SaveCameraLocation2"), IE_Pressed, this, &ARTSPlayerController::SaveCameraLocationWithIndex<2>);
    InputComponent->BindAction(TEXT("SaveCameraLocation3"), IE_Pressed, this, &ARTSPlayerController::SaveCameraLocationWithIndex<3>);
    InputComponent->BindAction(TEXT("SaveCameraLocation4"), IE_Pressed, this, &ARTSPlayerController::SaveCameraLocationWithIndex<4>);

    InputComponent->BindAction(TEXT("LoadCameraLocation0"), IE_Released, this, &ARTSPlayerController::LoadCameraLocationWithIndex<0>);
    InputComponent->BindAction(TEXT("LoadCameraLocation1"), IE_Released, this, &ARTSPlayerController::LoadCameraLocationWithIndex<1>);
    InputComponent->BindAction(TEXT("LoadCameraLocation2"), IE_Released, this, &ARTSPlayerController::LoadCameraLocationWithIndex<2>);
    InputComponent->BindAction(TEXT("LoadCameraLocation3"), IE_Released, this, &ARTSPlayerController::LoadCameraLocationWithIndex<3>);
    InputComponent->BindAction(TEXT("LoadCameraLocation4"), IE_Released, this, &ARTSPlayerController::LoadCameraLocationWithIndex<4>);

	InputComponent->BindAction(TEXT("ShowConstructionProgressBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingConstructionProgressBars);
	InputComponent->BindAction(TEXT("ShowConstructionProgressBars"), IE_Released, this, &ARTSPlayerController::StopShowingConstructionProgressBars);
	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingHealthBars);
	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Released, this, &ARTSPlayerController::StopShowingHealthBars);
	InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingProductionProgressBars);
	InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Released, this, &ARTSPlayerController::StopShowingProductionProgressBars);

	InputComponent->BindAction(TEXT("ConfirmBuildingPlacement"), IE_Released, this, &ARTSPlayerController::ConfirmBuildingPlacement);
	InputComponent->BindAction(TEXT("CancelBuildingPlacement"), IE_Released, this, &ARTSPlayerController::CancelBuildingPlacement);

	InputComponent->BindAction(TEXT("CancelConstruction"), IE_Released, this, &ARTSPlayerController::CancelConstruction);
	InputComponent->BindAction(TEXT("CancelProduction"), IE_Released, this, &ARTSPlayerController::CancelProduction);

	// Get camera bounds.
	for (TActorIterator<ARTSCameraBoundsVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		CameraBoundsVolume = *ActorItr;
		break;
	}

	if (!CameraBoundsVolume)
	{
		UE_LOG(LogRTS, Warning, TEXT("No RTSCameraBoundsVolume found. Camera will be able to move anywhere."));
	}

	// Setup control groups and camera locations.
	ControlGroups.SetNum(10);
    CameraLocations.SetNum(5);
}

void ARTSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARTSPlayerController::OnPlayerStateAvailable(ARTSPlayerState* NewPlayerState)
{
    // Discover own actors.
    ARTSPlayerState* RTSPlayerState = GetPlayerState();

    if (IsValid(RTSPlayerState))
    {
        RTSPlayerState->DiscoverOwnActors();
    }

    // Notify listeners.
    ReceiveOnPlayerStateAvailable(RTSPlayerState);
}

AActor* ARTSPlayerController::GetHoveredActor() const
{
	return HoveredActor;
}

ARTSPlayerState* ARTSPlayerController::GetPlayerState() const
{
	return Cast<ARTSPlayerState>(PlayerState);
}

TArray<AActor*> ARTSPlayerController::GetSelectedActors() const
{
	return SelectedActors;
}

bool ARTSPlayerController::GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults) const
{
	// Get ray.
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
}

bool ARTSPlayerController::GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults) const
{
	// Get ray.
	FVector WorldOrigin = FVector(WorldPositionXY.X, WorldPositionXY.Y, HitResultTraceDistance / 2);
	FVector WorldDirection = -FVector::UpVector;

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
}

bool ARTSPlayerController::GetSelectionFrame(FIntRect& OutSelectionFrame) const
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

    float MinX = FMath::Min(SelectionFrameMouseStartPosition.X, MouseX);
    float MaxX = FMath::Max(SelectionFrameMouseStartPosition.X, MouseX);
    float MinY = FMath::Min(SelectionFrameMouseStartPosition.Y, MouseY);
    float MaxY = FMath::Max(SelectionFrameMouseStartPosition.Y, MouseY);

	OutSelectionFrame = FIntRect(FIntPoint(MinX, MinY), FIntPoint(MaxX, MaxY));

	return true;
}

ARTSTeamInfo* ARTSPlayerController::GetTeamInfo() const
{
	ARTSPlayerState* CurrentPlayerState = Cast<ARTSPlayerState>(PlayerState);

	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetTeam();
	}
	
	return nullptr;
}

bool ARTSPlayerController::IssueOrder(const FRTSOrderData& Order)
{
    ERTSOrderGroupExecutionType GroupExecutionType = URTSOrderLibrary::GetOrderGroupExecutionType(Order.OrderClass);

    bool bSuccess = false;

    for (auto SelectedActor : SelectedActors)
    {
        APawn* SelectedPawn = Cast<APawn>(SelectedActor);

        if (!IsValid(SelectedPawn))
        {
            continue;
        }

        if (SelectedPawn->GetOwner() != this)
        {
            continue;
        }

        FRTSOrderTargetData TargetData = URTSOrderLibrary::GetOrderTargetData(SelectedActor, Order.TargetActor, Order.TargetLocation);

        if (!URTSOrderLibrary::IsValidOrderTarget(Order.OrderClass, SelectedActor, TargetData, Order.Index))
        {
            continue;
        }

        // Send order to server.
        ServerIssueOrder(SelectedPawn, Order);

        if (IsNetMode(NM_Client))
        {
            // Notify listeners.
            NotifyOnIssuedOrder(SelectedPawn, Order);
        }

        if (GroupExecutionType == ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_Any)
        {
            // Just send a single actor.
            return true;
        }

        bSuccess = true;
    }

    return bSuccess;
}

void ARTSPlayerController::ServerIssueOrder_Implementation(APawn* OrderedPawn, const FRTSOrderData& Order)
{
    auto PawnController = Cast<ARTSPawnAIController>(OrderedPawn->GetController());

    if (!PawnController)
    {
        return;
    }

    // Issue order.
    PawnController->IssueOrder(Order);

    // Notify listeners.
    NotifyOnIssuedOrder(OrderedPawn, Order);
}

bool ARTSPlayerController::ServerIssueOrder_Validate(APawn* OrderedPawn, const FRTSOrderData& Order)
{
    // Verify owner to prevent cheating.
    return OrderedPawn->GetOwner() == this;
}

bool ARTSPlayerController::GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults) const
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

	return GetObjectsAtScreenPosition(MousePosition, OutHitResults);
}

bool ARTSPlayerController::GetObjectsInSelectionFrame(TArray<FHitResult>& HitResults) const
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

	// Iterate all actors.
	HitResults.Reset();

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		FVector2D ActorScreenPosition;

		if (UGameplayStatics::ProjectWorldToScreen(this, Actor->GetActorLocation(), ActorScreenPosition))
		{
			if (SelectionFrame.Contains(FIntPoint(ActorScreenPosition.X, ActorScreenPosition.Y)))
			{
				FHitResult HitResult(Actor, nullptr, Actor->GetActorLocation(), FVector());
				HitResults.Add(HitResult);
			}
		}
	}

	return HitResults.Num() > 0;
}

bool ARTSPlayerController::TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults) const
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);

	return World->LineTraceMultiByObjectType(
		OutHitResults,
		WorldOrigin,
		WorldOrigin + WorldDirection * HitResultTraceDistance,
		Params);
}

bool ARTSPlayerController::IsSelectableActor(AActor* Actor) const
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

void ARTSPlayerController::IssueDefaultOrder()
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
    TOptional<FVector> TargetLocation;

	for (auto& HitResult : HitResults)
	{
        TargetLocation = HitResult.Location;

		if (HitResult.Actor != nullptr)
		{
			// Issue attack order.
			if (IssueAttackOrder(HitResult.Actor.Get()))
			{
				return;
			}
			
			// Issue gather order.
			if (IssueGatherOrder(HitResult.Actor.Get()))
			{
				return;
			}

			// Issue construct order.
			if (IssueContinueConstructionOrder(HitResult.Actor.Get()))
			{
				return;
			}

            ALandscape* Landscape = Cast<ALandscape>(HitResult.Actor.Get());

            if (Landscape != nullptr)
            {
                // Issue move order.
                IssueMoveOrder(HitResult.Location);
                return;
            }
		}
	}

    if (TargetLocation.IsSet())
    {
        // Issue move order.
        IssueMoveOrder(TargetLocation.GetValue());
    }
}

bool ARTSPlayerController::GetSelectedSubgroupActorAndIndex(AActor** OutSelectedSubgroupActor, int32* OutSelectedSubgroupActorIndex)
{
    *OutSelectedSubgroupActor = nullptr;
    *OutSelectedSubgroupActorIndex = -1;

    if (SelectedActors.Num() <= 0)
    {
        return false;
    }

    for (int32 Index = 0; Index < SelectedActors.Num(); ++Index)
    {
        AActor* SelectedActor = SelectedActors[Index];

        if (!IsValid(SelectedActors[Index]))
        {
            continue;
        }

        if (SelectedActor->GetClass() == GetSelectedSubgroup())
        {
            *OutSelectedSubgroupActor = SelectedActor;
            *OutSelectedSubgroupActorIndex = Index;
            return true;
        }
    }

    // Selected subgroup invalid.
    return false;
}

void ARTSPlayerController::SelectNextSubgroupInDirection(int32 Sign)
{
    // Find first actor in selected subgroup.
    AActor* OldSelectedSubgroupActor = nullptr;
    int32 OldSelectedSubgroupActorIndex = 0;

    if (!GetSelectedSubgroupActorAndIndex(&OldSelectedSubgroupActor, &OldSelectedSubgroupActorIndex))
    {
        SelectFirstSubgroup();
        return;
    }

    // Iterate all other selected actors, wrapping around.
    AActor* NewSelectedSubgroupActor = nullptr;
    int NewSelectedSubgroupActorIndex = OldSelectedSubgroupActorIndex + Sign;

    while (SelectedActors.IsValidIndex(NewSelectedSubgroupActorIndex))
    {
        if (SelectedActors[NewSelectedSubgroupActorIndex]->GetClass() != SelectedSubgroup)
        {
            NewSelectedSubgroupActor = SelectedActors[NewSelectedSubgroupActorIndex];
            break;
        }

        NewSelectedSubgroupActorIndex += Sign;
    }

    if (!IsValid(NewSelectedSubgroupActor))
    {
        NewSelectedSubgroupActorIndex = Sign >= 0 ? 0 : SelectedActors.Num() - 1;

        while (NewSelectedSubgroupActorIndex != OldSelectedSubgroupActorIndex)
        {
            if (SelectedActors[NewSelectedSubgroupActorIndex]->GetClass() != SelectedSubgroup)
            {
                NewSelectedSubgroupActor = SelectedActors[NewSelectedSubgroupActorIndex];
                break;
            }

            NewSelectedSubgroupActorIndex += Sign;
        }
    }

    // Select new subgroup.
    if (IsValid(NewSelectedSubgroupActor))
    {
        SelectSubgroup(NewSelectedSubgroupActor->GetClass());
    }
}

bool ARTSPlayerController::IssueAttackOrder(AActor* Target)
{
    FRTSOrderData AttackOrder;
    AttackOrder.OrderClass = URTSAttackOrder::StaticClass();
    AttackOrder.TargetActor = Target;

	return IssueOrder(AttackOrder);
}

bool ARTSPlayerController::IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
    // Determine index.
    int32 BuildingIndex = INDEX_NONE;

    for (auto SelectedActor : SelectedActors)
    {
        if (!IsValid(SelectedActor) || SelectedActor->GetOwner() != this)
        {
            continue;
        }

        BuildingIndex = URTSConstructionLibrary::GetConstructableBuildingIndex(SelectedActor, BuildingClass);

        if (BuildingIndex != INDEX_NONE)
        {
            break;
        }
    }

    if (BuildingIndex == INDEX_NONE) 
    {
        return false;
    }

    FRTSOrderData BeginConstructionOrder;
    BeginConstructionOrder.OrderClass = URTSBeginConstructionOrder::StaticClass();
    BeginConstructionOrder.TargetLocation = TargetLocation;
    BeginConstructionOrder.Index = BuildingIndex;

    return IssueOrder(BeginConstructionOrder);
}

bool ARTSPlayerController::IssueContinueConstructionOrder(AActor* ConstructionSite)
{
    FRTSOrderData ContinueConstructionOrder;
    ContinueConstructionOrder.OrderClass = URTSContinueConstructionOrder::StaticClass();
    ContinueConstructionOrder.TargetActor = ConstructionSite;

    return IssueOrder(ContinueConstructionOrder);
}

bool ARTSPlayerController::IssueGatherOrder(AActor* ResourceSource)
{
    FRTSOrderData GatherOrder;
    GatherOrder.OrderClass = URTSGatherOrder::StaticClass();
    GatherOrder.TargetActor = ResourceSource;

    return IssueOrder(GatherOrder);
}

bool ARTSPlayerController::IssueMoveOrder(const FVector& TargetLocation)
{
    FRTSOrderData MoveOrder;
    MoveOrder.OrderClass = URTSMoveOrder::StaticClass();
    MoveOrder.TargetLocation = TargetLocation;

    return IssueOrder(MoveOrder);
}

AActor* ARTSPlayerController::GetSelectedProductionActorFor(TSubclassOf<AActor> ProductClass) const
{
    // Find suitable selected actor.
    for (auto SelectedActor : SelectedActors)
    {
        if (!IsValid(SelectedActor))
        {
            continue;
        }

        // Verify owner.
        if (SelectedActor->GetOwner() != this)
        {
            continue;
        }

        // Check if production actor.
        auto ProductionComponent = SelectedActor->FindComponentByClass<URTSProductionComponent>();

        if (!ProductionComponent)
        {
            continue;
        }

        if (!ProductionComponent->GetAvailableProducts().Contains(ProductClass))
        {
            continue;
        }

        return SelectedActor;
    }

    return nullptr;
}

bool ARTSPlayerController::CheckCanIssueProductionOrder(TSubclassOf<AActor> ProductClass)
{
    AActor* SelectedActor = GetSelectedProductionActorFor(ProductClass);

    if (!IsValid(SelectedActor))
    {
        return true;
    }

    // Check costs.
    URTSProductionCostComponent* ProductionCostComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

    if (ProductionCostComponent && !PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->GetResources()))
    {
        NotifyOnErrorOccurred(TEXT("Not enough resources."));
        return false;
    }

    // Check requirements.
    TSubclassOf<AActor> MissingRequirement;

    if (URTSGameplayLibrary::GetMissingRequirementFor(this, SelectedActor, ProductClass, MissingRequirement))
    {
        URTSNameComponent* NameComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSNameComponent>(MissingRequirement);

        if (NameComponent)
        {
            FString ErrorMessage = TEXT("Missing requirement: ");
            ErrorMessage.Append(NameComponent->GetName().ToString());
            NotifyOnErrorOccurred(ErrorMessage);
        }
        else
        {
            NotifyOnErrorOccurred("Missing requirement.");
        }

        return false;
    }

    return true;
}

void ARTSPlayerController::IssueProductionOrder(TSubclassOf<AActor> ProductClass)
{
    AActor* SelectedActor = GetSelectedProductionActorFor(ProductClass);

    if (!IsValid(SelectedActor))
    {
        return;
    }

    // Begin production.
    ServerStartProduction(SelectedActor, ProductClass);

    if (IsNetMode(NM_Client))
    {
        UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to start production %s."), *SelectedActor->GetName(), *ProductClass->GetName());

        // Notify listeners.
        NotifyOnIssuedProductionOrder(SelectedActor, ProductClass);
    }
}

void ARTSPlayerController::IssueStopOrder()
{
    FRTSOrderData StopOrder;
    StopOrder.OrderClass = URTSStopOrder::StaticClass();
    
    IssueOrder(StopOrder);
}

void ARTSPlayerController::SelectActors(TArray<AActor*> Actors, ERTSSelectionCameraFocusMode CameraFocusMode)
{
    // Check for double-selection.
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    const float CurrentSelectionTime = World->GetRealTimeSeconds();
    const float SelectionTimeDelta = CurrentSelectionTime - LastSelectionTime;
    LastSelectionTime = CurrentSelectionTime;

    if (CameraFocusMode == ERTSSelectionCameraFocusMode::SELECTIONFOCUS_FocusOnDoubleSelection &&
        DoubleGroupSelectionTime > 0.0f && SelectionTimeDelta < DoubleGroupSelectionTime)
    {
        // Compare groups.
        if (Actors.Num() == SelectedActors.Num())
        {
            bool bSameSelection = true;

            for (AActor* Actor : Actors)
            {
                if (!SelectedActors.Contains(Actor))
                {
                    bSameSelection = false;
                    break;
                }
            }

            if (bSameSelection)
            {
                // Focus selected actors instead.
                if (SelectedActors.Num() > 0)
                {
                    FocusCameraOnActor(SelectedActors[0]);
                }

                return;
            }
        }
    }

	// Clear selection.
	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->DeselectActor();
		}
	}

    // Sort by priority and lifetime.
    Actors.Sort([=](const AActor& Lhs, const AActor& Rhs) {
        const URTSSelectableComponent* FirstSelectableComponent = Lhs.FindComponentByClass<URTSSelectableComponent>();
        const URTSSelectableComponent* SecondSelectableComponent = Rhs.FindComponentByClass<URTSSelectableComponent>();

        if (!IsValid(FirstSelectableComponent) || !IsValid(SecondSelectableComponent))
        {
            return true;
        }

        if (FirstSelectableComponent->GetSelectionPriority() > SecondSelectableComponent->GetSelectionPriority())
        {
            return true;
        }

        if (FirstSelectableComponent->GetSelectionPriority() < SecondSelectableComponent->GetSelectionPriority())
        {
            return false;
        }

        if (URTSGameplayLibrary::IsReadyToUse(&Lhs) && !URTSGameplayLibrary::IsReadyToUse(&Rhs))
        {
            return true;
        }

        if (!URTSGameplayLibrary::IsReadyToUse(&Lhs) && URTSGameplayLibrary::IsReadyToUse(&Rhs))
        {
            return false;
        }

        return Lhs.CreationTime < Rhs.CreationTime;
    });

	// Apply new selection.
	SelectedActors = Actors;

	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->SelectActor();

            // Play selection sound.
            if (SelectionSoundCooldownRemaining <= 0.0f &&
                URTSGameplayLibrary::IsOwnedByLocalPlayer(SelectedActor) &&
                IsValid(SelectableComponent->GetSelectedSound()))
            {
                UGameplayStatics::PlaySound2D(this, SelectableComponent->GetSelectedSound());
                SelectionSoundCooldownRemaining = SelectableComponent->GetSelectedSound()->GetDuration();
            }
		}
	}

    // Initially, select first subgroup.
    SelectFirstSubgroup();

	// Notify listeners.
	NotifyOnSelectionChanged(SelectedActors);
}

TSubclassOf<AActor> ARTSPlayerController::GetSelectedSubgroup() const
{
    return SelectedSubgroup;
}

AActor* ARTSPlayerController::GetSelectedSubgroupActor()
{
    AActor* SelectedSubgroupActor;
    int32 SelectedSubgroupActorIndex;
    return GetSelectedSubgroupActorAndIndex(&SelectedSubgroupActor, &SelectedSubgroupActorIndex) ? SelectedSubgroupActor
        : nullptr;
}

void ARTSPlayerController::GetSelectedSubgroupActors(TArray<AActor*>& OutActors) const
{
    for (AActor* Actor : SelectedActors)
    {
        if (!IsValid(Actor))
        {
            continue;
        }

        if (Actor->GetClass() == SelectedSubgroup)
        {
            OutActors.Add(Actor);
        }
    }
}

void ARTSPlayerController::SelectFirstSubgroup()
{
    if (SelectedActors.Num() > 0)
    {
        SelectSubgroup(SelectedActors[0]->GetClass());
    }
    else
    {
        SelectSubgroup(nullptr);
    }
}

void ARTSPlayerController::SelectNextSubgroup()
{
    SelectNextSubgroupInDirection(+1);
}

void ARTSPlayerController::SelectPreviousSubgroup()
{
    SelectNextSubgroupInDirection(-1);
}

void ARTSPlayerController::SelectSubgroup(TSubclassOf<AActor> NewSubgroup)
{
    if (SelectedSubgroup == NewSubgroup)
    {
        return;
    }

    SelectedSubgroup = NewSubgroup;

    if (NewSubgroup == nullptr)
    {
        UE_LOG(LogRTS, Verbose, TEXT("Cleared selected subgroup."));
    }
    else
    {
        UE_LOG(LogRTS, Verbose, TEXT("Selected subgroup %s."), *NewSubgroup->GetName());
    }

    // Notify listeners.
    NotifyOnSelectedSubgroupChanged(NewSubgroup);
}

void ARTSPlayerController::SaveControlGroup(int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	// Save control group.
    FRTSControlGroup ControlGroup;
    ControlGroup.Actors = SelectedActors;
	ControlGroups[Index] = ControlGroup;

	UE_LOG(LogRTS, Log, TEXT("Saved selection to control group %d."), Index);
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

void ARTSPlayerController::LoadControlGroup(int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	SelectActors(ControlGroups[Index].Actors, ERTSSelectionCameraFocusMode::SELECTIONFOCUS_FocusOnDoubleSelection);

	UE_LOG(LogRTS, Log, TEXT("Loaded selection from control group %d."), Index);
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

void ARTSPlayerController::FocusCameraOnLocation(FVector2D NewCameraLocation)
{
    APawn* PlayerPawn = GetPawnOrSpectator();

    if (!IsValid(PlayerPawn))
    {
        return;
    }

    // Calculate where to put the camera, considering its angle, to center on the specified location.
    FVector FinalCameraLocation = FVector(NewCameraLocation.X - GetCameraDistance(), NewCameraLocation.Y, 0.0f);

    // Enforce camera bounds.
    if (IsValid(CameraBoundsVolume) && !CameraBoundsVolume->EncompassesPoint(FinalCameraLocation))
    {
        UBrushComponent* CameraBoundsBrushComponent = CameraBoundsVolume->GetBrushComponent();
        FTransform CameraBoundsBrushTransform = CameraBoundsBrushComponent->GetComponentTransform();
        FBoxSphereBounds Bounds = CameraBoundsBrushComponent->CalcBounds(CameraBoundsBrushTransform);

        FinalCameraLocation.X = FMath::Clamp(FinalCameraLocation.X, Bounds.GetBox().Min.X, Bounds.GetBox().Max.X);
        FinalCameraLocation.Y = FMath::Clamp(FinalCameraLocation.Y, Bounds.GetBox().Min.Y, Bounds.GetBox().Max.Y);
    }

    // Keep camera height.
    FinalCameraLocation.Z = PlayerPawn->GetActorLocation().Z;

    // Update camera location.
    PlayerPawn->SetActorLocation(FinalCameraLocation);
}

void ARTSPlayerController::FocusCameraOnActor(AActor* Actor)
{
    TArray<AActor*> Actors;
    Actors.Add(Actor);
    FocusCameraOnActors(Actors);
}

void ARTSPlayerController::FocusCameraOnActors(TArray<AActor*> Actors)
{
    // Get center of group.
    FVector2D Locations = FVector2D::ZeroVector;
    int32 NumActors = 0;

    for (auto Actor : Actors)
    {
        if (!IsValid(Actor))
        {
            continue;
        }

        FVector ActorLocation = Actor->GetActorLocation();
        Locations.X += ActorLocation.X;
        Locations.Y += ActorLocation.Y;

        ++NumActors;
    }

    FVector2D Location = Locations / NumActors;
    FocusCameraOnLocation(Location);
}

void ARTSPlayerController::SaveCameraLocation(int32 Index)
{
    APawn* PlayerPawn = GetPawnOrSpectator();

    if (!IsValid(PlayerPawn))
    {
        return;
    }

    if (!CameraLocations.IsValidIndex(Index))
    {
        return;
    }

    // Save camera location.
    CameraLocations[Index] = PlayerPawn->GetActorLocation();

    UE_LOG(LogRTS, Log, TEXT("Saved camera location to index %d."), Index);
}

void ARTSPlayerController::LoadCameraLocation(int32 Index)
{
    APawn* PlayerPawn = GetPawnOrSpectator();

    if (!IsValid(PlayerPawn))
    {
        return;
    }

    if (!CameraLocations.IsValidIndex(Index))
    {
        return;
    }

    // Restore camera.
    PlayerPawn->SetActorLocation(CameraLocations[Index]);

    UE_LOG(LogRTS, Log, TEXT("Loaded camera location with index %d."), Index);
}

bool ARTSPlayerController::IsConstructionProgressBarHotkeyPressed() const
{
	return bConstructionProgressBarHotkeyPressed;
}

bool ARTSPlayerController::IsHealthBarHotkeyPressed() const
{
	return bHealthBarHotkeyPressed;
}

bool ARTSPlayerController::IsProductionProgressBarHotkeyPressed() const
{
	return bProductionProgressBarHotkeyPressed;
}

bool ARTSPlayerController::CheckCanBeginBuildingPlacement(TSubclassOf<AActor> BuildingClass)
{
    // Check resources.
    URTSConstructionSiteComponent* ConstructionSiteComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSConstructionSiteComponent>(BuildingClass);

    if (ConstructionSiteComponent && !PlayerResourcesComponent->CanPayAllResources(ConstructionSiteComponent->GetConstructionCosts()))
    {
        NotifyOnErrorOccurred(TEXT("Not enough resources."));
        return false;
    }

    // Check requirements.
    if (SelectedActors.Num() > 0)
    {
        TSubclassOf<AActor> MissingRequirement;

        if (URTSGameplayLibrary::GetMissingRequirementFor(this, SelectedActors[0], BuildingClass, MissingRequirement))
        {
            URTSNameComponent* NameComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSNameComponent>(MissingRequirement);

            if (NameComponent)
            {
                FString ErrorMessage = TEXT("Missing requirement: ");
                ErrorMessage.Append(NameComponent->GetName().ToString());
                NotifyOnErrorOccurred(ErrorMessage);
            }
            else
            {
                NotifyOnErrorOccurred("Missing requirement.");
            }

            return false;
        }
    }
    
    return true;
}

void ARTSPlayerController::BeginBuildingPlacement(TSubclassOf<AActor> BuildingClass)
{
    // Spawn preview building.
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    BuildingCursor = GetWorld()->SpawnActor<ARTSBuildingCursor>(BuildingCursorClass, SpawnParams);

    if (IsValid(BuildingCursor))
    {
        BuildingCursor->SetupForBuilding(BuildingClass);
    }

	BuildingBeingPlacedClass = BuildingClass;

	UE_LOG(LogRTS, Log, TEXT("Beginning placement of building %s."), *BuildingClass->GetName());

	// Notify listeners.
	NotifyOnBuildingPlacementStarted(BuildingClass);
}

bool ARTSPlayerController::CanPlaceBuilding_Implementation(TSubclassOf<AActor> BuildingClass, const FVector& Location) const
{
    if (IsValid(BuildingCursor) && BuildingCursor->HasGrid())
    {
        return BuildingCursor->AreAllCellsValid();
    }

    UWorld* World = GetWorld();
    return URTSCollisionLibrary::IsSuitableLocationForActor(World, BuildingClass, Location);
}

void ARTSPlayerController::Surrender()
{
    if (IsNetMode(NM_Client))
    {
        UE_LOG(LogRTS, Log, TEXT("%s surrenders the game."), *GetName());
    }

    ServerSurrender();
}

void ARTSPlayerController::GameHasEnded(class AActor* EndGameFocus /*= NULL*/, bool bIsWinner /*= false*/)
{
    ClientGameHasEnded(bIsWinner);
}

void ARTSPlayerController::ClientGameHasEnded_Implementation(bool bIsWinner)
{
    NotifyOnGameHasEnded(bIsWinner);
}

void ARTSPlayerController::StartSelectActors()
{
	if (BuildingCursor)
	{
		// We're selecting a building location instead.
		return;
	}

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

				UE_LOG(LogRTS, Log, TEXT("Deselected actor %s."), *HitResult.Actor->GetName());
			}
			else if (!ActorsToSelect.Contains(HitResult.Actor))
			{
				// Select actor.
				ActorsToSelect.Add(HitResult.Actor.Get());

				UE_LOG(LogRTS, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
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

			UE_LOG(LogRTS, Log, TEXT("Selected actor %s."), *HitResult.Actor->GetName());
		}
    }

	SelectActors(ActorsToSelect, ERTSSelectionCameraFocusMode::SELECTIONFOCUS_DoNothing);

	bCreatingSelectionFrame = false;
}

void ARTSPlayerController::StartShowingConstructionProgressBars()
{
	bConstructionProgressBarHotkeyPressed = true;
}

void ARTSPlayerController::StopShowingConstructionProgressBars()
{
	bConstructionProgressBarHotkeyPressed = false;
}

void ARTSPlayerController::StartShowingHealthBars()
{
	bHealthBarHotkeyPressed = true;
}

void ARTSPlayerController::StopShowingHealthBars()
{
	bHealthBarHotkeyPressed = false;
}

void ARTSPlayerController::StartShowingProductionProgressBars()
{
	bProductionProgressBarHotkeyPressed = true;
}

void ARTSPlayerController::StopShowingProductionProgressBars()
{
	bProductionProgressBarHotkeyPressed = false;
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

	if (!CanPlaceBuilding(BuildingBeingPlacedClass, HoveredWorldPosition))
	{
		UE_LOG(LogRTS, Log, TEXT("Can't place building %s at %s."), *BuildingBeingPlacedClass->GetName(), *HoveredWorldPosition.ToString());

		// Notify listeners.
		NotifyOnBuildingPlacementError(BuildingBeingPlacedClass, HoveredWorldPosition);
		return;
	}

	UE_LOG(LogRTS, Log, TEXT("Placed building %s at %s."), *BuildingBeingPlacedClass->GetName(), *HoveredWorldPosition.ToString());

	// Remove dummy building.
	BuildingCursor->Destroy();
	BuildingCursor = nullptr;

	// Notify listeners.
	NotifyOnBuildingPlacementConfirmed(BuildingBeingPlacedClass, HoveredWorldPosition);

	// Start construction.
	IssueBeginConstructionOrder(BuildingBeingPlacedClass, HoveredWorldPosition);
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

	UE_LOG(LogRTS, Log, TEXT("Cancelled placement of building %s."), *BuildingBeingPlacedClass->GetName());

	// Notify listeners.
	NotifyOnBuildingPlacementCancelled(BuildingBeingPlacedClass);
}

void ARTSPlayerController::CancelConstruction()
{
	for (auto SelectedActor : SelectedActors)
	{
		// Verify construction site and owner.
		auto ConstructionSiteComponent = SelectedActor->FindComponentByClass<URTSConstructionSiteComponent>();

		if (!ConstructionSiteComponent)
		{
			continue;
		}

		if (SelectedActor->GetOwner() != this)
		{
			continue;
		}

		// Send message to server.
		ServerCancelConstruction(SelectedActor);

		// Only cancel one construction at a time.
		return;
	}
}

void ARTSPlayerController::ServerCancelConstruction_Implementation(AActor* ConstructionSite)
{
	auto ConstructionSiteComponent = ConstructionSite->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent)
	{
		return;
	}

	// Cancel construction.
	ConstructionSiteComponent->CancelConstruction();
}

bool ARTSPlayerController::ServerCancelConstruction_Validate(AActor* ConstructionSite)
{
	// Verify owner to prevent cheating.
	return ConstructionSite->GetOwner() == this;
}

void ARTSPlayerController::CancelProduction()
{
	for (auto SelectedActor : SelectedActors)
	{
		// Verify production actor.
		auto ProductionComponent = SelectedActor->FindComponentByClass<URTSProductionComponent>();

		if (!ProductionComponent)
		{
			continue;
		}

		if (SelectedActor->GetOwner() != this)
		{
			continue;
		}

		if (!URTSGameplayLibrary::IsReadyToUse(SelectedActor))
		{
			continue;
		}

		// Send message to server.
		ServerCancelProduction(SelectedActor);

		// Only cancel one production at a time.
		return;
	}
}

void ARTSPlayerController::ServerCancelProduction_Implementation(AActor* ProductionActor)
{
	auto ProductionComponent = ProductionActor->FindComponentByClass<URTSProductionComponent>();

	if (!ProductionComponent)
	{
		return;
	}

	// Cancel production.
	ProductionComponent->CancelProduction();
}

bool ARTSPlayerController::ServerCancelProduction_Validate(AActor* ProductionActor)
{
	// Verify owner to prevent cheating.
	return ProductionActor->GetOwner() == this;
}

void ARTSPlayerController::ServerStartProduction_Implementation(AActor* ProductionActor, TSubclassOf<AActor> ProductClass)
{
	auto ProductionComponent = ProductionActor->FindComponentByClass<URTSProductionComponent>();

	if (!IsValid(ProductionComponent))
	{
		return;
	}

	ProductionComponent->StartProduction(ProductClass);
}

bool ARTSPlayerController::ServerStartProduction_Validate(AActor* ProductionActor, TSubclassOf<AActor> ProductClass)
{
	// Verify owner to prevent cheating.
	return ProductionActor->GetOwner() == this;
}

void ARTSPlayerController::ServerSurrender_Implementation()
{
    UE_LOG(LogRTS, Log, TEXT("%s surrenders the game."), *GetName());

    // Notify game mode.
    ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(this));

    if (GameMode != nullptr)
    {
        GameMode->NotifyOnPlayerDefeated(this);
    }
}

bool ARTSPlayerController::ServerSurrender_Validate()
{
    return true;
}

void ARTSPlayerController::MoveCameraLeftRight(float Value)
{
    CameraLeftRightAxisValue = Value;
}

void ARTSPlayerController::MoveCameraUpDown(float Value)
{
    CameraUpDownAxisValue = Value;
}

void ARTSPlayerController::ZoomCamera(float Value)
{
    CameraZoomAxisValue = Value;
}

float ARTSPlayerController::GetCameraDistance() const
{
    APawn* PlayerPawn = GetPawnOrSpectator();

    if (!IsValid(PlayerPawn))
    {
        return 0.0f;
    }

    UCameraComponent* Camera = PlayerPawn->FindComponentByClass<UCameraComponent>();

    if (!IsValid(Camera))
    {
        return 0.0f;
    }

    // Get camera angle.
    float CameraAngle = Camera->GetRelativeRotation().Pitch;

    if (CameraAngle < 0.0f)
    {
        CameraAngle += 90.0f;
    }

    // Get camera distance using trigonometry.
    // We are assuming that the terrain is flat, centered at the origin, and the camera has no roll or yaw.
    return Camera->GetRelativeLocation().Z * FMath::Tan(FMath::DegreesToRadians(CameraAngle));
}

void ARTSPlayerController::NotifyOnActorOwnerChanged(AActor* Actor)
{
	ReceiveOnActorOwnerChanged(Actor);
}

void ARTSPlayerController::NotifyOnBuildingPlacementStarted(TSubclassOf<AActor> BuildingClass)
{
	ReceiveOnBuildingPlacementStarted(BuildingClass);
}

void ARTSPlayerController::NotifyOnBuildingPlacementConfirmed(TSubclassOf<AActor> BuildingClass, const FVector& Location)
{
	ReceiveOnBuildingPlacementConfirmed(BuildingClass, Location);
}

void ARTSPlayerController::NotifyOnBuildingPlacementError(TSubclassOf<AActor> BuildingClass, const FVector& Location)
{
	ReceiveOnBuildingPlacementError(BuildingClass, Location);
}

void ARTSPlayerController::NotifyOnBuildingPlacementCancelled(TSubclassOf<AActor> BuildingClass)
{
	ReceiveOnBuildingPlacementCancelled(BuildingClass);
}

void ARTSPlayerController::NotifyOnErrorOccurred(const FString& ErrorMessage)
{
    ReceiveOnErrorOccurred(ErrorMessage);
}

void ARTSPlayerController::NotifyOnGameHasEnded(bool bIsWinner)
{
    ReceiveOnGameHasEnded(bIsWinner);
}

void ARTSPlayerController::NotifyOnIssuedOrder(APawn* OrderedPawn, const FRTSOrderData& Order)
{
    ReceiveOnIssuedOrder(OrderedPawn, Order);

    if (Order.OrderClass == URTSAttackOrder::StaticClass())
    {
        NotifyOnIssuedAttackOrder(OrderedPawn, Order.TargetActor);
    }
    else if (Order.OrderClass == URTSBeginConstructionOrder::StaticClass())
    {
        TSubclassOf<AActor> BuildingClass = URTSConstructionLibrary::GetConstructableBuildingClass(OrderedPawn, Order.Index);
        NotifyOnIssuedBeginConstructionOrder(OrderedPawn, BuildingClass, Order.TargetLocation);
    }
    else if (Order.OrderClass == URTSContinueConstructionOrder::StaticClass())
    {
        NotifyOnIssuedContinueConstructionOrder(OrderedPawn, Order.TargetActor);
    }
    else if (Order.OrderClass == URTSGatherOrder::StaticClass())
    {
        NotifyOnIssuedGatherOrder(OrderedPawn, Order.TargetActor);
    }
    else if (Order.OrderClass == URTSMoveOrder::StaticClass())
    {
        NotifyOnIssuedMoveOrder(OrderedPawn, Order.TargetLocation);
    }
    else if (Order.OrderClass == URTSStopOrder::StaticClass())
    {
        NotifyOnIssuedStopOrder(OrderedPawn);
    }
}

void ARTSPlayerController::NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target)
{
	ReceiveOnIssuedAttackOrder(OrderedPawn, Target);
}

void ARTSPlayerController::NotifyOnIssuedBeginConstructionOrder(APawn* OrderedPawn, TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	ReceiveOnIssuedBeginConstructionOrder(OrderedPawn, BuildingClass, TargetLocation);
}

void ARTSPlayerController::NotifyOnIssuedContinueConstructionOrder(APawn* OrderedPawn, AActor* ConstructionSite)
{
	ReceiveOnIssuedContinueConstructionOrder(OrderedPawn, ConstructionSite);
}

void ARTSPlayerController::NotifyOnIssuedGatherOrder(APawn* OrderedPawn, AActor* ResourceSource)
{
	ReceiveOnIssuedGatherOrder(OrderedPawn, ResourceSource);
}

void ARTSPlayerController::NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation)
{
    ReceiveOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

void ARTSPlayerController::NotifyOnIssuedProductionOrder(AActor* OrderedActor, TSubclassOf<AActor> ProductClass)
{
    ReceiveOnIssuedProductionOrder(OrderedActor, ProductClass);
}

void ARTSPlayerController::NotifyOnIssuedStopOrder(APawn* OrderedPawn)
{
	ReceiveOnIssuedStopOrder(OrderedPawn);
}

void ARTSPlayerController::NotifyOnSelectionChanged(const TArray<AActor*>& Selection)
{
    ReceiveOnSelectionChanged(Selection);
}

void ARTSPlayerController::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		// Notify listeners that new vision info is available now.
		ARTSVisionInfo* VisionInfo = ARTSVisionInfo::GetVisionInfoForTeam(GetWorld(), NewTeam->GetTeamIndex());
		NotifyOnVisionInfoAvailable(VisionInfo);
	}
}

void ARTSPlayerController::NotifyOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo)
{
	// On server side, we're only interested in our own vision info.
	// Other player controllers that exist on the server for replication to the clients
	// should not affect rendering vision for the local player.
	if (this != GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	// Setup fog of war.
    ARTSGameState* GameState = Cast<ARTSGameState>(GetWorld()->GetGameState());

    if (IsValid(GameState))
    {
        ARTSVisionManager* VisionManager = GameState->GetVisionManager();

        if (IsValid(VisionManager))
        {
            VisionManager->SetLocalVisionInfo(VisionInfo);
        }
    }

	// Allow others to setup vision.
	ReceiveOnVisionInfoAvailable(VisionInfo);
}

void ARTSPlayerController::NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition)
{
	APawn* PlayerPawn = GetPawnOrSpectator();

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

void ARTSPlayerController::NotifyOnSelectedSubgroupChanged(TSubclassOf<AActor> Subgroup)
{
    ReceiveOnSelectedSubgroupChanged(Subgroup);
}

void ARTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Update sound cooldowns.
    if (SelectionSoundCooldownRemaining > 0.0f)
    {
        SelectionSoundCooldownRemaining -= DeltaTime;
    }

    APawn* PlayerPawn = GetPawnOrSpectator();

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

    // Apply zoom input.
    UCameraComponent* PlayerPawnCamera = PlayerPawn->FindComponentByClass<UCameraComponent>();

    if (IsValid(PlayerPawnCamera))
    {
        FVector CameraLocation = PlayerPawnCamera->GetRelativeLocation();
        CameraLocation.Z += CameraZoomSpeed * CameraZoomAxisValue * DeltaTime;
        CameraLocation.Z = FMath::Clamp(CameraLocation.Z, MinCameraDistance, MaxCameraDistance);
        PlayerPawnCamera->SetRelativeLocation(CameraLocation);
    }

	// Get hovered actors.
    AActor* OldHoveredActor = HoveredActor;
	HoveredActor = nullptr;

	TArray<FHitResult> HitResults;

	if (GetObjectsAtPointerPosition(HitResults))
	{
		for (auto& HitResult : HitResults)
		{
            // Store hovered world position.
            HoveredWorldPosition = HitResult.Location;

            // Update position of building being placed.
            if (BuildingCursor)
            {
                BuildingCursor->SetCursorLocation(HoveredWorldPosition);

                bool bLocationValid = CanPlaceBuilding(BuildingBeingPlacedClass, HoveredWorldPosition);
                BuildingCursor->SetLocationValid(bLocationValid);
            }

			// Check if hit any actor.
			if (HitResult.Actor == nullptr || Cast<ALandscape>(HitResult.Actor.Get()) != nullptr)
			{
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

	// Verify selection.
	int32 DeselectedActors = SelectedActors.RemoveAll([=](AActor* SelectedActor) { return SelectedActor->IsHidden(); });

    if (DeselectedActors > 0)
    {
        // Notify listeners.
        NotifyOnSelectionChanged(SelectedActors);

        // Verify subgroup.
        if (SelectedActors.Num() > 0 && !IsValid(GetSelectedSubgroupActor()))
        {
            SelectFirstSubgroup();
        }
    }

    // Notify listeners.
    if (OldHoveredActor != HoveredActor)
    {
        if (IsValid(OldHoveredActor))
        {
            auto OldHoveredSelectableComponent = OldHoveredActor->FindComponentByClass<URTSSelectableComponent>();

            if (OldHoveredSelectableComponent)
            {
                OldHoveredSelectableComponent->UnhoverActor();
            }
        }

        if (IsValid(HoveredActor))
        {
            auto HoveredSelectableComponent = HoveredActor->FindComponentByClass<URTSSelectableComponent>();

            if (HoveredSelectableComponent)
            {
                HoveredSelectableComponent->HoverActor();
            }
        }
    }
}

void ARTSPlayerController::InitPlayerState()
{
    Super::InitPlayerState();

    if (!IsValid(PlayerState))
    {
        return;
    }

    UE_LOG(LogRTS, Log, TEXT("Player %s set up player state %s (%s)."), *GetName(), *PlayerState->GetName(),
        *PlayerState->GetPlayerName());

    OnPlayerStateAvailable(GetPlayerState());
}

void ARTSPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    if (!IsValid(PlayerState))
    {
        return;
    }

    UE_LOG(LogRTS, Log, TEXT("Player %s received player state %s (%s)."), *GetName(), *PlayerState->GetName(),
        *PlayerState->GetPlayerName());

    OnPlayerStateAvailable(GetPlayerState());
}
