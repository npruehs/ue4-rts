#include "RTSPluginPCH.h"
#include "RTSPlayerController.h"

#include "EngineUtils.h"
#include "Landscape.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"

#include "RTSAttackComponent.h"
#include "RTSAttackableComponent.h"
#include "RTSBuilderComponent.h"
#include "RTSBuildingCursor.h"
#include "RTSCameraBoundsVolume.h"
#include "RTSCharacter.h"
#include "RTSCharacterAIController.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSFogOfWarActor.h"
#include "RTSGathererComponent.h"
#include "RTSNameComponent.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerAdvantageComponent.h"
#include "RTSPlayerResourcesComponent.h"
#include "RTSPlayerState.h"
#include "RTSProductionComponent.h"
#include "RTSProductionCostComponent.h"
#include "RTSResourceSourceComponent.h"
#include "RTSSelectableComponent.h"
#include "RTSTeamInfo.h"
#include "RTSUtilities.h"
#include "RTSVisionInfo.h"


ARTSPlayerController::ARTSPlayerController()
{
    PlayerAdvantageComponent = CreateDefaultSubobject<URTSPlayerAdvantageComponent>(TEXT("Player Advantage"));
    PlayerResourcesComponent = CreateDefaultSubobject<URTSPlayerResourcesComponent>(TEXT("Player Resources"));
}

void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Allow immediate updates for interested listeners.
    for (int32 Index = 0; Index < PlayerResourcesComponent->ResourceTypes.Num(); ++Index)
    {
        PlayerResourcesComponent->OnResourcesChanged.Broadcast(
            PlayerResourcesComponent->ResourceTypes[Index],
            0.0f,
            PlayerResourcesComponent->ResourceAmounts[Index],
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

	InputComponent->BindAction(TEXT("IssueOrder"), IE_Released, this, &ARTSPlayerController::IssueOrder);
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

	InputComponent->BindAction(TEXT("ShowConstructionProgressBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingConstructionProgressBars);
	InputComponent->BindAction(TEXT("ShowConstructionProgressBars"), IE_Released, this, &ARTSPlayerController::StopShowingConstructionProgressBars);
	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingHealthBars);
	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Released, this, &ARTSPlayerController::StopShowingHealthBars);
	InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingProductionProgressBars);
	InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Released, this, &ARTSPlayerController::StopShowingProductionProgressBars);

	InputComponent->BindAction(TEXT("BeginDefaultBuildingPlacement"), IE_Released, this, &ARTSPlayerController::BeginDefaultBuildingPlacement);
	InputComponent->BindAction(TEXT("ConfirmBuildingPlacement"), IE_Released, this, &ARTSPlayerController::ConfirmBuildingPlacement);
	InputComponent->BindAction(TEXT("CancelBuildingPlacement"), IE_Released, this, &ARTSPlayerController::CancelBuildingPlacement);

	InputComponent->BindAction(TEXT("CancelConstruction"), IE_Released, this, &ARTSPlayerController::CancelConstruction);

	InputComponent->BindAction(TEXT("StartDefaultProduction"), IE_Released, this, &ARTSPlayerController::StartDefaultProduction);
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

	// Setup control groups.
	ControlGroups.SetNum(10);
}

void ARTSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

bool ARTSPlayerController::GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults)
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

bool ARTSPlayerController::GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults)
{
	// Get ray.
	FVector WorldOrigin = FVector(WorldPositionXY.X, WorldPositionXY.Y, HitResultTraceDistance / 2);
	FVector WorldDirection = -FVector::UpVector;

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
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

ARTSTeamInfo* ARTSPlayerController::GetTeamInfo() const
{
	ARTSPlayerState* CurrentPlayerState = Cast<ARTSPlayerState>(PlayerState);

	if (CurrentPlayerState)
	{
		return CurrentPlayerState->Team;
	}
	
	return nullptr;
}

bool ARTSPlayerController::GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults)
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

bool ARTSPlayerController::TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults)
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

			continue;
		}

		// Issue move order.
		IssueMoveOrder(HitResult.Location);
		return;
	}
}

bool ARTSPlayerController::IssueAttackOrder(AActor* Target)
{
	if (!Target)
	{
		return false;
	}

	if (!Target->FindComponentByClass<URTSAttackableComponent>())
	{
		return false;
	}

	ARTSTeamInfo* MyTeam = GetPlayerState()->Team;

	// Issue attack orders.
	bool bSuccess = false;

	for (auto SelectedActor : SelectedActors)
	{
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Verify target.
		auto TargetOwnerComponent = Target->FindComponentByClass<URTSOwnerComponent>();

		if (TargetOwnerComponent && TargetOwnerComponent->IsSameTeamAsActor(SelectedActor))
		{
			continue;
		}

		if (SelectedActor->FindComponentByClass<URTSAttackComponent>() == nullptr)
		{
			continue;
		}

		// Send attack order to server.
		ServerIssueAttackOrder(SelectedPawn, Target);

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to attack %s."), *SelectedActor->GetName(), *Target->GetName());
        }

		// Notify listeners.
		NotifyOnIssuedAttackOrder(SelectedPawn, Target);

		bSuccess = true;
	}

	return bSuccess;
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
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to attack %s."), *OrderedPawn->GetName(), *Target->GetName());

	// Notify listeners.
	NotifyOnIssuedAttackOrder(OrderedPawn, Target);
}

bool ARTSPlayerController::ServerIssueAttackOrder_Validate(APawn* OrderedPawn, AActor* Target)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

bool ARTSPlayerController::IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	// Find suitable selected builder.
	for (auto SelectedActor : SelectedActors)
	{
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Check if builder.
		auto BuilderComponent = SelectedPawn->FindComponentByClass<URTSBuilderComponent>();

		if (!BuilderComponent)
		{
			continue;
		}

		// Check if builder knows about building.
		if (!BuilderComponent->ConstructibleBuildingClasses.Contains(BuildingClass))
		{
			continue;
		}

		// Send construction order to server.
		ServerIssueBeginConstructionOrder(SelectedPawn, BuildingClass, TargetLocation);

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to begin constructing %s at %s."), *SelectedPawn->GetName(), *BuildingClass->GetName(), *TargetLocation.ToString());
        }

		// Notify listeners.
		NotifyOnIssuedBeginConstructionOrder(SelectedPawn, BuildingClass, TargetLocation);

		// Just send one builder.
		return true;
	}

	return false;
}

bool ARTSPlayerController::ServerIssueContinueConstructionOrder_Validate(APawn* OrderedPawn, AActor* ConstructionSite)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::ServerIssueBeginConstructionOrder_Implementation(APawn* OrderedPawn, TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue construction order.
	PawnController->IssueBeginConstructionOrder(BuildingClass, TargetLocation);
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to begin constructing %s at %s."), *OrderedPawn->GetName(), *BuildingClass->GetName(), *TargetLocation.ToString());

	// Notify listeners.
	NotifyOnIssuedBeginConstructionOrder(OrderedPawn, BuildingClass, TargetLocation);
}

bool ARTSPlayerController::IssueContinueConstructionOrder(AActor* ConstructionSite)
{
	if (!ConstructionSite)
	{
		return false;
	}

	auto ConstructionSiteComponent = ConstructionSite->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent || ConstructionSiteComponent->IsFinished())
	{
		return false;
	}

	ARTSTeamInfo* MyTeam = GetPlayerState()->Team;

	// Issue construction orders.
	bool bSuccess = false;

	for (auto SelectedActor : SelectedActors)
	{
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Verify target.
		auto TargetOwnerComponent = ConstructionSite->FindComponentByClass<URTSOwnerComponent>();

		if (TargetOwnerComponent && !TargetOwnerComponent->IsSameTeamAsActor(SelectedActor))
		{
			continue;
		}

		if (SelectedActor->FindComponentByClass<URTSBuilderComponent>() == nullptr)
		{
			continue;
		}

		// Send construction order to server.
		ServerIssueContinueConstructionOrder(SelectedPawn, ConstructionSite);

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to continue constructing %s."), *SelectedActor->GetName(), *ConstructionSite->GetName());
        }

		// Notify listeners.
		NotifyOnIssuedContinueConstructionOrder(SelectedPawn, ConstructionSite);

		bSuccess = true;
	}

	return bSuccess;
}

bool ARTSPlayerController::IssueGatherOrder(AActor* ResourceSource)
{
	if (!ResourceSource)
	{
		return false;
	}

	auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

	if (!ResourceSourceComponent)
	{
		return false;
	}

	// Issue gather orders.
	bool bSuccess = false;

	for (auto SelectedActor : SelectedActors)
	{
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Verify gatherer.
		auto GathererComponent = SelectedActor->FindComponentByClass<URTSGathererComponent>();
		if (!GathererComponent || !GathererComponent->CanGatherFrom(ResourceSource))
		{
			continue;
		}

		// Send gather order to server.
		ServerIssueGatherOrder(SelectedPawn, ResourceSource);

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to gather resources from %s."), *SelectedActor->GetName(), *ResourceSource->GetName());
        }

		// Notify listeners.
		NotifyOnIssuedGatherOrder(SelectedPawn, ResourceSource);

		bSuccess = true;
	}

	return bSuccess;
}

void ARTSPlayerController::ServerIssueGatherOrder_Implementation(APawn* OrderedPawn, AActor* ResourceSource)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue gather order.
	PawnController->IssueGatherOrder(ResourceSource);
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to gather resources from %s."), *OrderedPawn->GetName(), *ResourceSource->GetName());

	// Notify listeners.
	NotifyOnIssuedGatherOrder(OrderedPawn, ResourceSource);
}

bool ARTSPlayerController::ServerIssueGatherOrder_Validate(APawn* OrderedPawn, AActor* ResourceSourc)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::ServerIssueContinueConstructionOrder_Implementation(APawn* OrderedPawn, AActor* ConstructionSite)
{
	auto PawnController = Cast<ARTSCharacterAIController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue construction order.
	PawnController->IssueContinueConstructionOrder(ConstructionSite);
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to continue constructing %s."), *OrderedPawn->GetName(), *ConstructionSite->GetName());

	// Notify listeners.
	NotifyOnIssuedContinueConstructionOrder(OrderedPawn, ConstructionSite);
}

bool ARTSPlayerController::ServerIssueBeginConstructionOrder_Validate(APawn* OrderedPawn, TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

bool ARTSPlayerController::IssueMoveOrder(const FVector& TargetLocation)
{
    // Issue move orders.
	bool bSuccess = false;

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

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to move to %s."), *SelectedActor->GetName(), *TargetLocation.ToString());
        }

        // Notify listeners.
        NotifyOnIssuedMoveOrder(SelectedPawn, TargetLocation);

		bSuccess = true;
    }

	return bSuccess;
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
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to move to %s."), *OrderedPawn->GetName(), *TargetLocation.ToString());

	// Notify listeners.
	NotifyOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

bool ARTSPlayerController::ServerIssueMoveOrder_Validate(APawn* OrderedPawn, const FVector& TargetLocation)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

AActor* ARTSPlayerController::GetSelectedProductionActor()
{
    // Find suitable selected actor.
    for (auto SelectedActor : SelectedActors)
    {
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

        if (ProductionComponent->AvailableProducts.Num() <= 0)
        {
            continue;
        }

        return SelectedActor;
    }

    return nullptr;
}

bool ARTSPlayerController::CheckCanIssueProductionOrder(int32 ProductIndex)
{
    AActor* SelectedActor = GetSelectedProductionActor();

    if (!SelectedActor)
    {
        return true;
    }

    URTSProductionComponent* ProductionComponent = SelectedActor->FindComponentByClass<URTSProductionComponent>();

    if (ProductIndex >= ProductionComponent->AvailableProducts.Num())
    {
        return true;
    }

    TSubclassOf<AActor> ProductClass = ProductionComponent->AvailableProducts[ProductIndex];

    // Check costs.
    URTSProductionCostComponent* ProductionCostComponent = URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

    if (ProductionCostComponent && !PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->Resources))
    {
        NotifyOnErrorOccurred(TEXT("Not enough resources."));
        return false;
    }

    // Check requirements.
    TSubclassOf<AActor> MissingRequirement;

    if (URTSUtilities::GetMissingRequirementFor(this, SelectedActor, ProductClass, MissingRequirement))
    {
        URTSNameComponent* NameComponent = URTSUtilities::FindDefaultComponentByClass<URTSNameComponent>(MissingRequirement);

        if (NameComponent)
        {
            FString ErrorMessage = TEXT("Missing requirement: ");
            ErrorMessage.Append(NameComponent->Name.ToString());
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

void ARTSPlayerController::IssueProductionOrder(int32 ProductIndex)
{
    AActor* SelectedActor = GetSelectedProductionActor();

    if (!SelectedActor)
    {
        return;
    }

    // Begin production.
    ServerStartProduction(SelectedActor, ProductIndex);

    if (IsNetMode(NM_Client))
    {
        UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to start production %i."), *SelectedActor->GetName(), ProductIndex);
    }

    // Notify listeners.
    NotifyOnIssuedProductionOrder(SelectedActor, ProductIndex);
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

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to stop."), *SelectedActor->GetName());
        }

		// Notify listeners.
		NotifyOnIssuedStopOrder(SelectedPawn);
	}
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
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to stop."), *OrderedPawn->GetName());

	// Notify listeners.
	NotifyOnIssuedStopOrder(OrderedPawn);
}

bool ARTSPlayerController::ServerIssueStopOrder_Validate(APawn* OrderedPawn)
{
	// Verify owner to prevent cheating.
	return OrderedPawn->GetOwner() == this;
}

void ARTSPlayerController::SelectActors(TArray<AActor*> Actors)
{
	// Clear selection.
	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->DeselectActor();
		}
	}

	// Apply new selection.
	SelectedActors = Actors;

	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->SelectActor();
		}
	}

	// Notify listeners.
	NotifyOnSelectionChanged(SelectedActors);
}

void ARTSPlayerController::SaveControlGroup(int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	// Save control group.
	ControlGroups[Index] = SelectedActors;

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

	SelectActors(ControlGroups[Index]);

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

bool ARTSPlayerController::IsConstructionProgressBarHotkeyPressed()
{
	return bConstructionProgressBarHotkeyPressed;
}

bool ARTSPlayerController::IsHealthBarHotkeyPressed()
{
	return bHealthBarHotkeyPressed;
}

bool ARTSPlayerController::IsProductionProgressBarHotkeyPressed()
{
	return bProductionProgressBarHotkeyPressed;
}

bool ARTSPlayerController::CheckCanBeginBuildingPlacement(TSubclassOf<AActor> BuildingClass)
{
    // Check resources.
    URTSConstructionSiteComponent* ConstructionSiteComponent = URTSUtilities::FindDefaultComponentByClass<URTSConstructionSiteComponent>(BuildingClass);

    if (ConstructionSiteComponent && !PlayerResourcesComponent->CanPayAllResources(ConstructionSiteComponent->ConstructionCosts))
    {
        NotifyOnErrorOccurred(TEXT("Not enough resources."));
        return false;
    }

    // Check requirements.
    if (SelectedActors.Num() > 0)
    {
        TSubclassOf<AActor> MissingRequirement;

        if (URTSUtilities::GetMissingRequirementFor(this, SelectedActors[0], BuildingClass, MissingRequirement))
        {
            URTSNameComponent* NameComponent = URTSUtilities::FindDefaultComponentByClass<URTSNameComponent>(MissingRequirement);

            if (NameComponent)
            {
                FString ErrorMessage = TEXT("Missing requirement: ");
                ErrorMessage.Append(NameComponent->Name.ToString());
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
	// Spawn dummy building.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* DefaultBuilding = BuildingClass->GetDefaultObject<AActor>();
	USkeletalMeshComponent* SkeletalMeshComponent = DefaultBuilding->FindComponentByClass<USkeletalMeshComponent>();

	BuildingCursor = GetWorld()->SpawnActor<ARTSBuildingCursor>(BuildingCursorClass, SpawnParams);
	BuildingCursor->SetMesh(SkeletalMeshComponent->SkeletalMesh, DefaultBuilding->GetActorRelativeScale3D());
	BuildingCursor->SetInvalidLocation();

	BuildingBeingPlacedClass = BuildingClass;

	UE_LOG(LogRTS, Log, TEXT("Beginning placement of building %s."), *BuildingClass->GetName());

	// Notify listeners.
	NotifyOnBuildingPlacementStarted(BuildingClass);
}

bool ARTSPlayerController::CanPlaceBuilding_Implementation(TSubclassOf<AActor> BuildingClass, const FVector& Location) const
{
	UWorld* World = GetWorld();
    return URTSUtilities::IsSuitableLocationForActor(World, BuildingClass, Location);
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

	SelectActors(ActorsToSelect);

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

void ARTSPlayerController::BeginDefaultBuildingPlacement()
{
	// Find suitable selected builder.
	for (auto SelectedActor : SelectedActors)
	{
		// Verify owner.
		if (SelectedActor->GetOwner() != this)
		{
			continue;
		}

		// Check if builder.
		auto BuilderComponent = SelectedActor->FindComponentByClass<URTSBuilderComponent>();

		if (!BuilderComponent)
		{
			continue;
		}

		// Check if builder knows about building.
		if (BuilderComponent->ConstructibleBuildingClasses.Num() <= 0)
		{
			continue;
		}

		// Begin placement.
		BeginBuildingPlacement(BuilderComponent->ConstructibleBuildingClasses[0]);
		return;
	}
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

void ARTSPlayerController::StartDefaultProduction()
{
    IssueProductionOrder(0);
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

		if (!URTSUtilities::IsReadyToUse(SelectedActor))
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

void ARTSPlayerController::ServerStartProduction_Implementation(AActor* ProductionActor, int32 ProductIndex)
{
	auto ProductionComponent = ProductionActor->FindComponentByClass<URTSProductionComponent>();

	if (!ProductionComponent)
	{
		return;
	}

	if (ProductionComponent->AvailableProducts.Num() <= ProductIndex)
	{
		return;
	}

	// Begin production.
	TSubclassOf<AActor> ProductClass = ProductionComponent->AvailableProducts[ProductIndex];
	ProductionComponent->StartProduction(ProductClass);
}

bool ARTSPlayerController::ServerStartProduction_Validate(AActor* ProductionActor, int32 ProductIndex)
{
	// Verify owner to prevent cheating.
	return ProductionActor->GetOwner() == this;
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

void ARTSPlayerController::NotifyOnIssuedProductionOrder(AActor* OrderedActor, int32 ProductIndex)
{
    ReceiveOnIssuedProductionOrder(OrderedActor, ProductIndex);
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
		ARTSVisionInfo* VisionInfo = ARTSVisionInfo::GetVisionInfoForTeam(GetWorld(), NewTeam->TeamIndex);
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
	for (TActorIterator<ARTSFogOfWarActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		ARTSFogOfWarActor* FogOfWarActor = *ActorIt;
		FogOfWarActor->SetupVisionInfo(VisionInfo);
		break;
	}

	// Allow others to setup vision.
	ReceiveOnVisionInfoAvailable(VisionInfo);
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
			if (HitResult.Actor == nullptr || Cast<ALandscape>(HitResult.Actor.Get()) != nullptr)
			{
				// Store hovered world position.
				HoveredWorldPosition = HitResult.Location;

				// Update position of building being placed.
				if (BuildingCursor)
				{
					BuildingCursor->SetActorLocation(HoveredWorldPosition);

					if (CanPlaceBuilding(BuildingBeingPlacedClass, HoveredWorldPosition))
					{
						BuildingCursor->SetValidLocation();
					}
					else
					{
						BuildingCursor->SetInvalidLocation();
					}
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

	// Verify selection.
	SelectedActors.RemoveAll([=](AActor* SelectedActor) { return SelectedActor->bHidden; });
}
