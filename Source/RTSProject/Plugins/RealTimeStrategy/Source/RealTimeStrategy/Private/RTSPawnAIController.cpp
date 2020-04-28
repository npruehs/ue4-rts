#include "RTSPawnAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"


void ARTSPawnAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

	AttackComponent = InPawn->FindComponentByClass<URTSAttackComponent>();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;

	if (UseBlackboard(PawnBlackboardAsset, BlackboardComponent))
	{
		// Setup blackboard.
		IssueStopOrder();
	}

    // Run behavior tree.
    RunBehaviorTree(PawnBehaviorTreeAsset);
}

void ARTSPawnAIController::FindTargetInAcquisitionRadius()
{
	if (!AttackComponent)
	{
		return;
	}

	// Find nearby actors.
	TArray<struct FHitResult> HitResults;
	TraceSphere(GetPawn()->GetActorLocation(), AttackComponent->GetAcquisitionRadius(), GetPawn(), ECC_Pawn, HitResults);

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
		if (!URTSGameplayTagLibrary::HasGameplayTag(HitResult.Actor.Get(), URTSGameplayTagLibrary::Status_Permanent_CanBeAttacked()))
		{
			continue;
		}

		// Acquire target.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), HitResult.Actor.Get());

		UE_LOG(LogRTS, Log, TEXT("%s automatically acquired target %s."), *GetPawn()->GetName(), *HitResult.Actor->GetName());
		return;
	}
}

bool ARTSPawnAIController::HasOrder(ERTSOrderType OrderType) const
{
    return Blackboard->GetValueAsEnum(TEXT("OrderType")) == (uint8)OrderType;
}

bool ARTSPawnAIController::IsIdle() const
{
    return HasOrder(ERTSOrderType::ORDER_None);
}

void ARTSPawnAIController::IssueAttackOrder(AActor* Target)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	// Attack order already issued for this target
	if (Target == AttackComponent->GetTarget())
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

void ARTSPawnAIController::IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
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

	int32 BuildingIndex = BuilderComponent->GetConstructibleBuildingClasses().IndexOfByKey(BuildingClass);

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

void ARTSPawnAIController::IssueContinueConstructionOrder(AActor* ConstructionSite)
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

void ARTSPawnAIController::IssueGatherOrder(AActor* ResourceSource)
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

void ARTSPawnAIController::IssueMoveOrder(const FVector& Location)
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

void ARTSPawnAIController::IssueReturnResourcesOrder()
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

void ARTSPawnAIController::IssueStopOrder()
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

void ARTSPawnAIController::ApplyOrders()
{
    // Update behavior tree.
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}

    // Notify listeners.
    uint8 NewOrder = Blackboard->GetValueAsEnum(TEXT("OrderType"));
    OnOrderChanged.Broadcast((ERTSOrderType)NewOrder);
}

void ARTSPawnAIController::ClearBuildingClass()
{
	Blackboard->ClearValue(TEXT("BuildingClass"));
}

void ARTSPawnAIController::ClearHomeLocation()
{
	Blackboard->ClearValue(TEXT("HomeLocation"));
}

void ARTSPawnAIController::ClearTargetActor()
{
	Blackboard->ClearValue(TEXT("TargetActor"));
	AttackComponent->SetTarget(nullptr);
}

void ARTSPawnAIController::ClearTargetLocation()
{
	Blackboard->ClearValue(TEXT("TargetLocation"));
}

void ARTSPawnAIController::SetBuildingClass(int32 BuildingIndex)
{
	Blackboard->SetValueAsInt(TEXT("BuildingClass"), BuildingIndex);
}

void ARTSPawnAIController::SetHomeLocation(const FVector& HomeLocation)
{
	Blackboard->SetValueAsVector(TEXT("HomeLocation"), HomeLocation);
}

void ARTSPawnAIController::SetOrderType(const ERTSOrderType OrderType)
{
	Blackboard->SetValueAsEnum(TEXT("OrderType"), (uint8)OrderType);
}

void ARTSPawnAIController::SetTargetActor(AActor* TargetActor)
{
	Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetActor);
	AttackComponent->SetTarget(TargetActor);
}

void ARTSPawnAIController::SetTargetLocation(const FVector& TargetLocation)
{
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
}

bool ARTSPawnAIController::TraceSphere(
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

bool ARTSPawnAIController::VerifyBlackboard()
{
	if (!Blackboard)
	{
		UE_LOG(LogRTS, Warning, TEXT("Blackboard not set up for %s, can't receive orders. Check AI Controller Class and Auto Possess AI."), *GetPawn()->GetName());
		return false;
	}

	return true;
}
