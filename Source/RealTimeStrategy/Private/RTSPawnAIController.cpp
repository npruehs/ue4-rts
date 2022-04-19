#include "RTSPawnAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "Libraries/RTSConstructionLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Libraries/RTSOrderLibrary.h"
#include "Orders/RTSAttackOrder.h"
#include "Orders/RTSBeginConstructionOrder.h"
#include "Orders/RTSContinueConstructionOrder.h"
#include "Orders/RTSGatherOrder.h"
#include "Orders/RTSMoveOrder.h"
#include "Orders/RTSReturnResourcesOrder.h"
#include "Orders/RTSStopOrder.h"


void ARTSPawnAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CombatComponent = InPawn->FindComponentByClass<URTSCombatComponent>();

	// Make AI use assigned blackboard.

	if (UBlackboardComponent* BlackboardComponent; UseBlackboard(PawnBlackboardAsset, BlackboardComponent))
	{
		// Setup blackboard.
		IssueStopOrder();
	}

	// Run behavior tree.
	RunBehaviorTree(PawnBehaviorTreeAsset);
}

void ARTSPawnAIController::FindTargetInAcquisitionRadius()
{
	if (!IsValid(CombatComponent))
	{
		return;
	}

	// Find nearby actors.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetPawn());

	TArray<AActor*> NearbyActors;
	UKismetSystemLibrary::SphereOverlapActors(this, GetPawn()->GetActorLocation(),
	                                          CombatComponent->GetAcquisitionRadius(), AcquisitionObjectTypes, APawn::StaticClass(), ActorsToIgnore,
	                                          NearbyActors);

	// Find target to acquire.
	for (AActor* NearbyActor : NearbyActors)
	{
		if (!IsValid(NearbyActor))
		{
			continue;
		}

		// Check owner.
		const AActor* MyActor = GetPawn();

		if (IsValid(MyActor))
		{
			const URTSOwnerComponent* MyOwnerComponent = MyActor->FindComponentByClass<URTSOwnerComponent>();

			if (MyOwnerComponent && MyOwnerComponent->IsSameTeamAsActor(NearbyActor))
			{
				continue;
			}
		}

		// Check if found attackable actor.
		if (!URTSGameplayTagLibrary::HasGameplayTag(NearbyActor, URTSGameplayTagLibrary::Status_Permanent_CanBeAttacked()))
		{
			continue;
		}

		// Acquire target.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), NearbyActor);

		UE_LOG(LogRTS, Log, TEXT("%s automatically acquired target %s."), *GetPawn()->GetName(), *NearbyActor->GetName());
		return;
	}
}

TSubclassOf<URTSOrder> ARTSPawnAIController::GetCurrentOrder() const
{
	return Blackboard->GetValueAsClass(TEXT("OrderClass"));
}

void ARTSPawnAIController::ServerAddOrder_Implementation(const FRTSOrderData& Order)
{
	if (!Order.OrderClass)
	{
		return;
	}

	AddOrder(Order);
}

void ARTSPawnAIController::AddOrder(const FRTSOrderData& Order)
{
	Orders.Enqueue(Order);
}

void ARTSPawnAIController::InsertOrder(const FRTSOrderData& Order)
{
	TArray Data = {Order};

	while (!Orders.IsEmpty())
	{
		FRTSOrderData OrderData = *Orders.Peek();
		Data.Add(OrderData);
		Orders.Pop();
	}

	for (FRTSOrderData OrderData : Data)
	{
		Orders.Enqueue(OrderData);
	}

	ObtainNextOrder();
}

bool ARTSPawnAIController::HasOrder(ERTSOrderType OrderType) const
{
	UE_LOG(LogRTS, Warning, TEXT("ARTSPawnAIController::HasOrder has been deprecated as of plugin version 1.2. Please use HasOrderByClass instead."));
	return Blackboard->GetValueAsEnum(TEXT("OrderType")) == (uint8)OrderType;
}

bool ARTSPawnAIController::HasOrderByClass(TSubclassOf<URTSOrder> OrderClass) const
{
	return GetCurrentOrder() == OrderClass;
}

bool ARTSPawnAIController::IsIdle() const
{
	return HasOrderByClass(URTSStopOrder::StaticClass());
}

void ARTSPawnAIController::IssueOrder(const FRTSOrderData& Order)
{
	UE_LOG(LogRTS, Log, TEXT("Set order as %s"), *Order.OrderClass->GetName())
	Orders.Empty();
	AddOrder(Order);
	ObtainNextOrder();
}

void ARTSPawnAIController::ServerIssueOrder_Implementation(const FRTSOrderData& Order)
{
	if (!Order.OrderClass)
	{
		return;
	}
	UE_LOG(LogRTS, Log, TEXT("Got new Order %s"), *Order.OrderClass->GetName());
	IssueOrder(Order);
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

void ARTSPawnAIController::InsertContinueGathersOrder()
{
	const URTSGathererComponent* GathererComponent = GetPawn()->FindComponentByClass<URTSGathererComponent>();

	if (!GathererComponent)
	{
		return;
	}

	AActor* ResourceSource = GathererComponent->GetPreferredResourceSource();

	if (!ResourceSource)
	{
		return;
	}

	FRTSOrderData Order;
	Order.OrderClass  = URTSGatherOrder::StaticClass();
	Order.TargetActor = ResourceSource;
	InsertOrder(Order);
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
	const auto GathererComponent = GetPawn()->FindComponentByClass<URTSGathererComponent>();

	if (!GathererComponent)
	{
		return;
	}

	AActor* ResourceDrain = GathererComponent->FindClosestResourceDrain();

	if (!ResourceDrain)
	{
		return;
	}

	FRTSOrderData Order;
	Order.OrderClass = URTSReturnResourcesOrder::StaticClass();
	Order.TargetActor = ResourceDrain;
	IssueOrder(Order);
}

void ARTSPawnAIController::InsertReturnResourcesOrder()
{
	const auto GathererComponent = GetPawn()->FindComponentByClass<URTSGathererComponent>();

	if (!GathererComponent)
	{
		return;
	}

	AActor* ResourceDrain = GathererComponent->FindClosestResourceDrain();

	if (!ResourceDrain)
	{
		return;
	}

	FRTSOrderData Order;
	Order.OrderClass = URTSReturnResourcesOrder::StaticClass();
	Order.TargetActor = ResourceDrain;
	InsertOrder(Order);
}

void ARTSPawnAIController::IssueStopOrder()
{
	FRTSOrderData Order;
	Order.OrderClass = URTSStopOrder::StaticClass();
	IssueOrder(Order);
}

void ARTSPawnAIController::ObtainNextOrder()
{
	if (Orders.IsEmpty() || !Blackboard)
	{
		return;
	}

	const FRTSOrderData* OrderData = Orders.Peek();

	if (!OrderData)
	{
		return;
	}

	Blackboard->SetValueAsClass(TEXT("OrderClass"), OrderData->OrderClass);
	Blackboard->SetValueAsEnum(TEXT("OrderType"), (uint8)OrderData->GetOrderType());
	Blackboard->SetValueAsClass(TEXT("OrderClass"), OrderData->OrderClass);
	Blackboard->SetValueAsObject(TEXT("TargetActor"), OrderData->TargetActor);
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), OrderData->TargetLocation);

	Orders.Pop();
}

ERTSOrderType ARTSPawnAIController::OrderClassToType(UClass* OrderClass) const
{
	if (OrderClass == URTSAttackOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Attack;
	}
	if (OrderClass == URTSBeginConstructionOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_BeginConstruction;
	}
	if (OrderClass == URTSContinueConstructionOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_ContinueConstruction;
	}
	if (OrderClass == URTSGatherOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Gather;
	}
	if (OrderClass == URTSMoveOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_Move;
	}
	if (OrderClass == URTSReturnResourcesOrder::StaticClass())
	{
		return ERTSOrderType::ORDER_ReturnResources;
	}

	return ERTSOrderType::ORDER_None;
}
