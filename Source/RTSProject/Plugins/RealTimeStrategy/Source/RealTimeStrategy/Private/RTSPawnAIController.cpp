#include "RTSPawnAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "Libraries/RTSConstructionLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSAttackOrder.h"
#include "Orders/RTSBeginConstructionOrder.h"
#include "Orders/RTSContinueConstructionOrder.h"
#include "Orders/RTSGatherOrder.h"
#include "Orders/RTSMoveOrder.h"
#include "Orders/RTSStopOrder.h"


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

void ARTSPawnAIController::IssueOrder(const FRTSOrderData& Order)
{
    if (!VerifyBlackboard())
    {
        return;
    }

    // Update blackboard.
    ERTSOrderType OrderType = OrderClassToType(Order.OrderClass);

    SetOrderType(OrderType);
    SetOrderClass(Order.OrderClass);
    SetTargetActor(Order.TargetActor);
    SetTargetLocation(Order.TargetLocation);
    SetBuildingClass(Order.Index);

    if (OrderType == ERTSOrderType::ORDER_None)
    {
        SetHomeLocation(GetPawn()->GetActorLocation());
    }
    else
    {
        ClearHomeLocation();
    }

    // Stop any current orders and start over.
    ApplyOrders();
}

void ARTSPawnAIController::IssueAttackOrder(AActor* Target)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSAttackOrder::StaticClass();
    Order.TargetActor = Target;

    IssueOrder(Order);
}

void ARTSPawnAIController::IssueBeginConstructionOrder(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSBeginConstructionOrder::StaticClass();
    Order.Index = URTSConstructionLibrary::GetConstructableBuildingIndex(GetPawn(), BuildingClass);
    Order.TargetLocation = TargetLocation;

    IssueOrder(Order);
}

void ARTSPawnAIController::IssueContinueConstructionOrder(AActor* ConstructionSite)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSContinueConstructionOrder::StaticClass();
    Order.TargetActor = ConstructionSite;
    IssueOrder(Order);
}

void ARTSPawnAIController::IssueGatherOrder(AActor* ResourceSource)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSGatherOrder::StaticClass();
    Order.TargetActor = ResourceSource;
    IssueOrder(Order);
}

void ARTSPawnAIController::IssueMoveOrder(const FVector& Location)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSMoveOrder::StaticClass();
    Order.TargetLocation = Location;
    IssueOrder(Order);
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
    FRTSOrderData Order;
    Order.OrderClass = URTSStopOrder::StaticClass();
    IssueOrder(Order);
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
    UClass* NewOrderClass = Blackboard->GetValueAsClass(TEXT("OrderClass"));
    ERTSOrderType NewOrderType = OrderClassToType(NewOrderClass);

    OnOrderChanged.Broadcast(GetOwner(), NewOrderType);
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

void ARTSPawnAIController::SetOrderClass(UClass* OrderClass)
{
    Blackboard->SetValueAsClass(TEXT("OrderClass"), OrderClass);
}

void ARTSPawnAIController::SetOrderType(const ERTSOrderType OrderType)
{
	Blackboard->SetValueAsEnum(TEXT("OrderType"), (uint8)OrderType);
}

void ARTSPawnAIController::SetTargetActor(AActor* TargetActor)
{
	Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetActor);
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

ERTSOrderType ARTSPawnAIController::OrderClassToType(UClass* OrderClass) const
{
    if (OrderClass == URTSAttackOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_Attack;
    }
    else if (OrderClass == URTSBeginConstructionOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_BeginConstruction;
    }
    else if (OrderClass == URTSContinueConstructionOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_ContinueConstruction;
    }
    else if (OrderClass == URTSGatherOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_Gather;
    }
    else if (OrderClass == URTSMoveOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_Move;
    }

    return ERTSOrderType::ORDER_None;
}
