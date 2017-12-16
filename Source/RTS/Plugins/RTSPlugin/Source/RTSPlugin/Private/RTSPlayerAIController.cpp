#include "RTSPluginPCH.h"
#include "RTSPlayerAIController.h"

#include "EngineUtils.h"
#include "Landscape.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

#include "RTSBuilderComponent.h"
#include "RTSCharacterAIController.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSPlayerResourcesComponent.h"
#include "RTSProductionComponent.h"
#include "RTSProductionCostComponent.h"
#include "RTSUtilities.h"


ARTSPlayerAIController::ARTSPlayerAIController()
{
    PlayerResourcesComponent = CreateDefaultSubobject<URTSPlayerResourcesComponent>(TEXT("Player Resources"));

    bWantsPlayerState = true;

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

bool ARTSPlayerAIController::CanPayFor(TSubclassOf<APawn> PawnClass) const
{
    URTSProductionCostComponent* ProductionCostComponent = URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(PawnClass);

    if (ProductionCostComponent)
    {
        return PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->Resources);
    }

    URTSConstructionSiteComponent* ConstructionSiteComponent = URTSUtilities::FindDefaultComponentByClass<URTSConstructionSiteComponent>(PawnClass);

    if (ConstructionSiteComponent)
    {
        return PlayerResourcesComponent->CanPayAllResources(ConstructionSiteComponent->ConstructionCosts);
    }

    return true;
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

        if (!ProductionComponent->AvailableProducts.Contains(PawnClass))
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

        if (!BuilderComponent->ConstructibleBuildingClasses.Contains(PawnClass))
        {
            continue;
        }

        ARTSCharacterAIController* PawnController = Cast<ARTSCharacterAIController>(SomePawn->GetController());

        if (!PawnController)
        {
            continue;
        }

        // Find suitable building location: Get random nearby location.
        FVector TargetLocation = OwnBuilding != nullptr ? OwnBuilding->GetActorLocation() : SomePawn->GetActorLocation();
        TargetLocation.X += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);
        TargetLocation.Y += FMath::FRandRange(-MaximumBaseBuildingDistance, MaximumBaseBuildingDistance);
        TargetLocation.Z = 10000.0f;

        // Cast ray to hit world.
        FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);
        TArray<FHitResult> HitResults;

        World->LineTraceMultiByObjectType(
            HitResults,
            TargetLocation,
            FVector(TargetLocation.X, TargetLocation.Y, -TargetLocation.Z),
            Params);

        for (auto& HitResult : HitResults)
        {
            if (HitResult.Actor != nullptr)
            {
                ALandscape* Landscape = Cast<ALandscape>(HitResult.Actor.Get());

                if (Landscape != nullptr)
                {
                    TargetLocation = HitResult.Location;
                    break;;
                }

                continue;
            }

            TargetLocation = HitResult.Location;
            break;
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
