#include "RTSPlayerAIController.h"

#include "EngineUtils.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

#include "RTSPawnAIController.h"
#include "RTSLog.h"
#include "RTSPlayerState.h"
#include "Construction/RTSBuilderComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Economy/RTSPlayerResourcesComponent.h"
#include "Economy/RTSResourceSourceComponent.h"
#include "Economy/RTSResourceDrainComponent.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSGameplayLibrary.h"
#include "Orders/RTSBeginConstructionOrder.h"
#include "Orders/RTSContinueConstructionOrder.h"
#include "Production/RTSProductionComponent.h"
#include "Production/RTSProductionCostComponent.h"


ARTSPlayerAIController::ARTSPlayerAIController(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PlayerResourcesComponent = CreateDefaultSubobject<URTSPlayerResourcesComponent>(TEXT("Player Resources"));

	bWantsPlayerState = true;

	// Set reasonable default values.
	MaximumBaseBuildingDistance = 1500.0f;
}

TSubclassOf<AActor> ARTSPlayerAIController::GetNextPawnToProduce() const
{
	// Count own actors.
	TMap<TSubclassOf<AActor>, int32> OwnPawns;

	const ARTSPlayerState* RTSPlayerState = GetPlayerState<ARTSPlayerState>();
	TArray<AActor*> OwnActors = RTSPlayerState->GetOwnActors();

	for (const AActor* OwnActor : OwnActors)
	{
		int32& NumOwnedPawns = OwnPawns.FindOrAdd(OwnActor->GetClass());
		++NumOwnedPawns;

		if (const URTSProductionComponent* ProductionComponent = OwnActor->FindComponentByClass<URTSProductionComponent>(); IsValid(ProductionComponent))
		{
			ProductionComponent->GetProduction(OwnPawns);
		}
	}

	// TODO(np): Also count actors already in construction.

	// Check build order.
	TMap<TSubclassOf<AActor>, int32> BuildOrderPawns;
	for (TSubclassOf<AActor> PawnClass : RTSPlayerState->GetRace()->GetBuildOrder())
	{
		int32& NumRequiredPawns = BuildOrderPawns.FindOrAdd(PawnClass);
		++NumRequiredPawns;

		if (NumRequiredPawns > OwnPawns.FindRef(PawnClass))
		{
			return PawnClass;
		}
	}

	return nullptr;
}

AActor* ARTSPlayerAIController::GetPrimaryResourceDrain() const
{
	APawn* PrimaryResourceDrain = nullptr;

	for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
	{
		APawn* SomePawn = *PawnItr;

		if (SomePawn->GetOwner() != this)
		{
			continue;
		}

		if (SomePawn->FindComponentByClass<URTSResourceDrainComponent>() == nullptr)
		{
			continue;
		}

		return SomePawn;
	}

	return nullptr;
}

AActor* ARTSPlayerAIController::GetPrimaryResourceSource() const
{
	// Get resource drain.
	const AActor* PrimaryResourceDrain = GetPrimaryResourceDrain();

	if (PrimaryResourceDrain == nullptr)
	{
		return nullptr;
	}

	// Sweep for sources.
	AActor* ClosestResourceSource = nullptr;
	float ClosestResourceSourceDistance = 0.0f;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		const auto ResourceSource = *ActorItr;

		// Check if found resource source.
		const auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

		if (!ResourceSourceComponent)
		{
			continue;
		}

		// Check resource type.
		if (ResourceSourceComponent->GetResourceType() != PrimaryResourceType)
		{
			continue;
		}

		// Check distance.
		const float Distance = PrimaryResourceDrain->GetDistanceTo(ResourceSource);

		if (!ClosestResourceSource || Distance < ClosestResourceSourceDistance)
		{
			ClosestResourceSource = ResourceSource;
			ClosestResourceSourceDistance = Distance;
		}
	}

	return ClosestResourceSource;
}

bool ARTSPlayerAIController::CanPayFor(TSubclassOf<AActor> PawnClass) const
{
	const URTSProductionCostComponent* ProductionCostComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(PawnClass);

	if (ProductionCostComponent)
	{
		return PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->GetResources());
	}

	const URTSConstructionSiteComponent* ConstructionSiteComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSConstructionSiteComponent>(PawnClass);

	if (ConstructionSiteComponent)
	{
		return PlayerResourcesComponent->CanPayAllResources(ConstructionSiteComponent->GetConstructionCosts());
	}

	return true;
}

bool ARTSPlayerAIController::MeetsAllRequirementsFor(TSubclassOf<AActor> PawnClass) const
{
	AActor* AnyOwnActor = GetPrimaryResourceDrain();
	return URTSGameplayLibrary::OwnerMeetsAllRequirementsFor(AnyOwnActor, AnyOwnActor, PawnClass);
}

bool ARTSPlayerAIController::StartProduction(TSubclassOf<AActor> PawnClass)
{
	TArray<AActor*> OwnActors = GetPlayerState<ARTSPlayerState>()->GetOwnActors();

	// Get any own building location.
	const AActor* OwnBuilding = nullptr;

	for (const AActor* Actor : OwnActors)
	{
		if (Actor->GetOwner() != this)
		{
			continue;
		}

		URTSProductionComponent* ProductionComponent = Actor->FindComponentByClass<URTSProductionComponent>();

		if (const URTSConstructionSiteComponent* ConstructionSiteComponent = Actor->FindComponentByClass<URTSConstructionSiteComponent>(); !ConstructionSiteComponent && !ProductionComponent)
		{
			continue;
		}

		OwnBuilding = Actor;

		if (!ProductionComponent->GetAvailableProducts().Contains(PawnClass))
		{
			continue;
		}

		UE_LOG(LogRTS, Log, TEXT("Start Production: %s"), *Actor->GetName())

		// Start production.
		ProductionComponent->StartProduction(PawnClass);
		return true;
	}

	if (const UWorld* World = GetWorld(); !World)
	{
		UE_LOG(LogRTS, Error, TEXT("No World found"))
		return false;
	}

	// Find suitable builder.
	for (const AActor* Actor : OwnActors)
	{
		const APawn* OwnedPawn = Cast<APawn>(Actor);

		if (!IsValid(OwnedPawn) || OwnedPawn->GetOwner() != this)
		{
			continue;
		}

		const URTSBuilderComponent* BuilderComponent = OwnedPawn->FindComponentByClass<URTSBuilderComponent>();

		if (!BuilderComponent)
		{
			continue;
		}

		if (!BuilderComponent->GetConstructibleBuildingClasses().Contains(PawnClass))
		{
			continue;
		}

		ARTSPawnAIController* PawnController = Cast<ARTSPawnAIController>(OwnedPawn->GetController());

		if (!PawnController)
		{
			continue;
		}

		if (PawnController->HasOrderByClass(URTSBeginConstructionOrder::StaticClass()) ||
			PawnController->HasOrderByClass(URTSContinueConstructionOrder::StaticClass()))
		{
			// Don't take builders away from constructing other buildings.
			continue;
		}

		// Find suitable building location: Get random nearby location.
		FVector TargetLocation = OwnBuilding != nullptr ? OwnBuilding->GetActorLocation() : OwnedPawn->GetActorLocation();
		TargetLocation.X += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);
		TargetLocation.Y += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);

		TargetLocation = URTSCollisionLibrary::GetGroundLocation(this, TargetLocation);

		// If there's a primary resource drain, prevent blocking its path.
		const AActor* PrimaryResourceSource = GetPrimaryResourceSource();
		const AActor* PrimaryResourceDrain = GetPrimaryResourceDrain();

		if (PrimaryResourceSource != nullptr &&
			PrimaryResourceDrain != nullptr &&
			FVector::DistSquaredXY(PrimaryResourceSource->GetActorLocation(), TargetLocation) <
			FVector::DistSquaredXY(PrimaryResourceSource->GetActorLocation(), PrimaryResourceDrain->GetActorLocation()))
		{
			continue;
		}

		// Issue construction order.
		PawnController->IssueBeginConstructionOrder(PawnClass, TargetLocation);
		return true;
	}

	return false;
}

bool ARTSPlayerAIController::GivesBounty() const
{
	return bGivesBounty;
}

void ARTSPlayerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Make AI use assigned blackboard.
	UBlackboardComponent* BlackboardComponent;

	if (!UseBlackboard(PlayerBlackboardAsset, BlackboardComponent))
	{
		UE_LOG(LogRTS, Warning, TEXT("Failed to set up blackboard for AI %s."), *GetName());
	}

	// Run behavior tree.
	RunBehaviorTree(PlayerBehaviorTreeAsset);
}
