#include "RTSPluginPrivatePCH.h"
#include "RTSCharacterAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RTSAttackComponent.h"
#include "RTSAttackableComponent.h"
#include "RTSBuilderComponent.h"
#include "RTSCharacter.h"
#include "RTSGathererComponent.h"
#include "RTSOwnerComponent.h"


void ARTSCharacterAIController::Possess(APawn* InPawn)
{
    Super::Possess(InPawn);

	AttackComponent = InPawn->FindComponentByClass<URTSAttackComponent>();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;

	if (UseBlackboard(CharacterBlackboardAsset, BlackboardComponent))
	{
		// Setup blackboard.
		IssueStopOrder();
	}

    // Run behavior tree.
    RunBehaviorTree(CharacterBehaviorTreeAsset);
}

void ARTSCharacterAIController::FindTargetInAcquisitionRadius()
{
	if (!AttackComponent)
	{
		return;
	}

	// Find nearby actors.
	TArray<struct FHitResult> HitResults;
	TraceSphere(GetPawn()->GetActorLocation(), AttackComponent->AcquisitionRadius, GetPawn(), ECC_Pawn, HitResults);

	// Find target to acquire.
	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor == nullptr)
		{
			continue;
		}

		if (HitResult.Actor == GetPawn())
		{
			continue;
		}
		
		// Check owner.
		auto MyActor = GetPawn();
		auto TargetActor = HitResult.Actor.Get();

		if (MyActor && TargetActor)
		{
			auto MyOwnerComponent = MyActor->FindComponentByClass<URTSOwnerComponent>();

			if (MyOwnerComponent && MyOwnerComponent->IsSameTeamAsActor(TargetActor))
			{
				continue;
			}
		}

		// Check if found attackable actor.
		auto AttackableComponent = HitResult.Actor->FindComponentByClass<URTSAttackableComponent>();

		if (!AttackableComponent)
		{
			continue;
		}

		// Acquire target.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), HitResult.Actor.Get());

		UE_LOG(RTSLog, Log, TEXT("%s automatically acquired target %s."), *GetPawn()->GetName(), *HitResult.Actor->GetName());
	}
}

void ARTSCharacterAIController::IssueAttackOrder(AActor* Target)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_Attack);
	ClearBuildingClass();
	ClearHomeLocation();
	SetTargetActor(Target);
	ClearTargetLocation();

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Somehow, classes are not properly serialized to blackboard values and back, so we're going to use the building index here instead.
	URTSBuilderComponent* BuilderComponent = GetPawn()->FindComponentByClass<URTSBuilderComponent>();

	if (!BuilderComponent)
	{
		return;
	}

	int32 BuildingIndex = BuilderComponent->ConstructibleBuildingClasses.IndexOfByKey(BuildingClass);

	if (BuildingIndex == INDEX_NONE)
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_BeginConstruction);
	SetBuildingClass(BuildingIndex);
	ClearHomeLocation();
	ClearTargetActor();
	SetTargetLocation(TargetLocation);

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueContinueConstructionOrder(AActor* ConstructionSite)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_ContinueConstruction);
	ClearBuildingClass();
	ClearHomeLocation();
	SetTargetActor(ConstructionSite);
	ClearTargetLocation();

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueGatherOrder(AActor* ResourceSource)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_Gather);
	ClearBuildingClass();
	ClearHomeLocation();
	SetTargetActor(ResourceSource);
	ClearTargetLocation();

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueMoveOrder(const FVector& Location)
{
	if (!VerifyBlackboard())
	{
		return;
	}

    // Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_Move);
	ClearBuildingClass();
	ClearHomeLocation();
	ClearTargetActor();
	SetTargetLocation(Location);

    // Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueReturnResourcesOrder()
{
	if (!VerifyBlackboard())
	{
		return;
	}

	auto GathererComponent = GetPawn()->FindComponentByClass<URTSGathererComponent>();

	if (!GathererComponent)
	{
		return;
	}

	AActor* ResourceDrain = GathererComponent->FindClosestResourceDrain();

	if (!ResourceDrain)
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_ReturnResources);
	ClearBuildingClass();
	ClearHomeLocation();
	SetTargetActor(ResourceDrain);
	ClearTargetLocation();

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::IssueStopOrder()
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Update blackboard.
	SetOrderType(ERTSOrderType::ORDER_None);
	ClearBuildingClass();
	SetHomeLocation(GetPawn()->GetActorLocation());
	ClearTargetActor();
	ClearTargetLocation();

	// Stop any current orders and start over.
	ApplyOrders();
}

void ARTSCharacterAIController::ApplyOrders()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void ARTSCharacterAIController::ClearBuildingClass()
{
	Blackboard->ClearValue(TEXT("BuildingClass"));
}

void ARTSCharacterAIController::ClearHomeLocation()
{
	Blackboard->ClearValue(TEXT("HomeLocation"));
}

void ARTSCharacterAIController::ClearTargetActor()
{
	Blackboard->ClearValue(TEXT("TargetActor"));
}

void ARTSCharacterAIController::ClearTargetLocation()
{
	Blackboard->ClearValue(TEXT("TargetLocation"));
}

void ARTSCharacterAIController::SetBuildingClass(int32 BuildingIndex)
{
	Blackboard->SetValueAsInt(TEXT("BuildingClass"), BuildingIndex);
}

void ARTSCharacterAIController::SetHomeLocation(const FVector& HomeLocation)
{
	Blackboard->SetValueAsVector(TEXT("HomeLocation"), HomeLocation);
}

void ARTSCharacterAIController::SetOrderType(const ERTSOrderType OrderType)
{
	Blackboard->SetValueAsEnum(TEXT("OrderType"), (uint8)OrderType);
}

void ARTSCharacterAIController::SetTargetActor(AActor* TargetActor)
{
	Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetActor);
}

void ARTSCharacterAIController::SetTargetLocation(const FVector& TargetLocation)
{
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
}

bool ARTSCharacterAIController::TraceSphere(
	const FVector& Location,
	const float Radius,
	AActor* ActorToIgnore,
	ECollisionChannel TraceChannel,
	TArray<struct FHitResult>& OutHitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	const FVector Start = Location;
	const FVector End = Location + FVector::ForwardVector * Radius;

	return World->SweepMultiByObjectType(
		OutHitResults,
		Start,
		End,
		FQuat(),
		FCollisionObjectQueryParams(TraceChannel),
		FCollisionShape::MakeSphere(Radius)
	);
}

bool ARTSCharacterAIController::VerifyBlackboard()
{
	if (!Blackboard)
	{
		UE_LOG(RTSLog, Warning, TEXT("Blackboard not set up for %s, can't receive orders. Check AI Controller Class and Auto Possess AI."), *GetPawn()->GetName());
		return false;
	}

	return true;
}
