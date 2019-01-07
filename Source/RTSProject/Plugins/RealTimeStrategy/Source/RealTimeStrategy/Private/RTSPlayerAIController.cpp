#include "RealTimeStrategyPCH.h"
#include "RTSPlayerAIController.h"

#include "EngineUtils.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

#include "RTSBuilderComponent.h"
#include "RTSCharacterAIController.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSPlayerResourcesComponent.h"
#include "RTSProductionComponent.h"
#include "RTSProductionCostComponent.h"
#include "RTSResourceSourceComponent.h"
#include "RTSResourceDrainComponent.h"
#include "RTSUtilities.h"


ARTSPlayerAIController::ARTSPlayerAIController()
{
    PlayerResourcesComponent = CreateDefaultSubobject<URTSPlayerResourcesComponent>(TEXT("Player Resources"));

    bWantsPlayerState = true;

	// Set reasonable default values.
    MaximumBaseBuildingDistance = 1500.0f;
}

TSubclassOf<APawn> ARTSPlayerAIController::GetNextPawnToProduce() const
{
    // Count own actors.
    TMap<TSubclassOf<APawn>, int32> OwnPawns;

    for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
    {
        APawn* SomePawn = *PawnItr;

        if (SomePawn->GetOwner() != this)
        {
            continue;
        }

        int32& NumOwnedPawns = OwnPawns.FindOrAdd(SomePawn->GetClass());
        ++NumOwnedPawns;
    }

    // TODO(np): Also count actors already in production/construction.

    // Check build order.
    TMap<TSubclassOf<APawn>, int32> BuildOrderPawns;
    for (TSubclassOf<APawn> PawnClass : BuildOrder)
    {
        int32& NumRequiredPawns = BuildOrderPawns.FindOrAdd(PawnClass);
        ++NumRequiredPawns;

        if (NumRequiredPawns > OwnPawns.FindRef(PawnClass))
        {
            return PawnClass;
        }
    }

    return APawn::StaticClass();
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
    AActor* PrimaryResourceDrain = GetPrimaryResourceDrain();

    if (PrimaryResourceDrain == nullptr)
    {
        return nullptr;
    }

    // Sweep for sources.
    AActor* ClosestResourceSource = nullptr;
    float ClosestResourceSourceDistance = 0.0f;

    for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        auto ResourceSource = *ActorItr;

        // Check if found resource source.
        auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

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
        float Distance = PrimaryResourceDrain->GetDistanceTo(ResourceSource);

        if (!ClosestResourceSource || Distance < ClosestResourceSourceDistance)
        {
            ClosestResourceSource = ResourceSource;
            ClosestResourceSourceDistance = Distance;
        }
    }

    return ClosestResourceSource;
}

bool ARTSPlayerAIController::CanPayFor(TSubclassOf<APawn> PawnClass) const
{
    URTSProductionCostComponent* ProductionCostComponent = URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(PawnClass);

    if (ProductionCostComponent)
    {
        return PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->GetResources());
    }

    URTSConstructionSiteComponent* ConstructionSiteComponent = URTSUtilities::FindDefaultComponentByClass<URTSConstructionSiteComponent>(PawnClass);

    if (ConstructionSiteComponent)
    {
        return PlayerResourcesComponent->CanPayAllResources(ConstructionSiteComponent->GetConstructionCosts());
    }

    return true;
}

bool ARTSPlayerAIController::MeetsAllRequirementsFor(TSubclassOf<APawn> PawnClass) const
{
    AActor* AnyOwnActor = GetPrimaryResourceDrain();
    return URTSUtilities::OwnerMeetsAllRequirementsFor(AnyOwnActor, AnyOwnActor, PawnClass);
}

bool ARTSPlayerAIController::StartProduction(TSubclassOf<APawn> PawnClass)
{
    // Find suitable factory.
    for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
    {
        APawn* SomePawn = *PawnItr;

        if (SomePawn->GetOwner() != this)
        {
            continue;
        }

        URTSProductionComponent* ProductionComponent = SomePawn->FindComponentByClass<URTSProductionComponent>();
        
        if (!ProductionComponent)
        {
            continue;
        }

        if (!ProductionComponent->GetAvailableProducts().Contains(PawnClass))
        {
            continue;
        }

        // Start production.
        ProductionComponent->StartProduction(PawnClass);
        return true;
    }

    // Get any own building location.
    APawn* OwnBuilding = nullptr;

    for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
    {
        APawn* SomePawn = *PawnItr;

        if (SomePawn->GetOwner() != this)
        {
            continue;
        }

        URTSConstructionSiteComponent* ConstructionSiteComponent = SomePawn->FindComponentByClass<URTSConstructionSiteComponent>();

        if (!ConstructionSiteComponent)
        {
            continue;
        }

        OwnBuilding = SomePawn;
        break;
    }

    // Find suitable builder.
    UWorld* World = GetWorld();

    if (!World)
    {
        return false;
    }

    for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
    {
        APawn* SomePawn = *PawnItr;

        if (SomePawn->GetOwner() != this)
        {
            continue;
        }

        URTSBuilderComponent* BuilderComponent = SomePawn->FindComponentByClass<URTSBuilderComponent>();

        if (!BuilderComponent)
        {
            continue;
        }

        if (!BuilderComponent->GetConstructibleBuildingClasses().Contains(PawnClass))
        {
            continue;
        }

        ARTSCharacterAIController* PawnController = Cast<ARTSCharacterAIController>(SomePawn->GetController());

        if (!PawnController)
        {
            continue;
        }

        if (PawnController->HasOrder(ERTSOrderType::ORDER_BeginConstruction) ||
            PawnController->HasOrder(ERTSOrderType::ORDER_ContinueConstruction))
        {
            // Don't take builders away from constructing other buildings.
            continue;
        }

        // Find suitable building location: Get random nearby location.
        FVector TargetLocation = OwnBuilding != nullptr ? OwnBuilding->GetActorLocation() : SomePawn->GetActorLocation();
        TargetLocation.X += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);
        TargetLocation.Y += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);
        
        TargetLocation = URTSUtilities::GetGroundLocation(this, TargetLocation);

        // If there's a primary resource drain, prevent blocking its path.
        AActor* PrimaryResourceSource = GetPrimaryResourceSource();
        AActor* PrimaryResourceDrain = GetPrimaryResourceDrain();

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

void ARTSPlayerAIController::Possess(APawn* InPawn)
{
    Super::Possess(InPawn);

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;

    if (!UseBlackboard(PlayerBlackboardAsset, BlackboardComponent))
    {
        UE_LOG(LogRTS, Warning, TEXT("Failed to set up blackboard for AI %s."), *GetName());
    }

    // Run behavior tree.
    RunBehaviorTree(PlayerBehaviorTreeAsset);
}
