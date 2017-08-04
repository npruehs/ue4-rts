#include "RTSPluginPrivatePCH.h"
#include "RTSCharacterAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RTSAttackComponent.h"
#include "RTSAttackableComponent.h"
#include "RTSCharacter.h"
#include "RTSOrderType.h"
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
		Blackboard->SetValueAsObject("TargetActor", HitResult.Actor.Get());

		UE_LOG(RTSLog, Log, TEXT("%s automatically acquired target %s."), *GetPawn()->GetName(), *HitResult.Actor->GetName());
	}
}

void ARTSCharacterAIController::IssueAttackOrder(AActor* Target)
{
	// Update blackboard.
	Blackboard->SetValueAsEnum("OrderType", ERTSOrderType::ORDER_Attack);
	Blackboard->ClearValue("HomeLocation");
	Blackboard->SetValueAsObject("TargetActor", Target);
	Blackboard->ClearValue("TargetLocation");

	// Stop any current orders and start over.
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void ARTSCharacterAIController::IssueMoveOrder(const FVector& Location)
{
    // Update blackboard.
	Blackboard->SetValueAsEnum("OrderType", ERTSOrderType::ORDER_Move);
	Blackboard->ClearValue("HomeLocation");
	Blackboard->ClearValue("TargetActor");
    Blackboard->SetValueAsVector("TargetLocation", Location);

    // Stop any current orders and start over.
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->RestartTree();
    }
}

void ARTSCharacterAIController::IssueStopOrder()
{
	// Update blackboard.
	Blackboard->SetValueAsEnum("OrderType", ERTSOrderType::ORDER_None);
	Blackboard->SetValueAsVector("HomeLocation", GetPawn()->GetActorLocation());
	Blackboard->ClearValue("TargetActor");
	Blackboard->ClearValue("TargetLocation");

	// Stop any current orders and start over.
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

bool ARTSCharacterAIController::TraceSphere(const FVector& Location, const float Radius, AActor* ActorToIgnore, ECollisionChannel TraceChannel, TArray<struct FHitResult>& OutHitResults)
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