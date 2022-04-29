// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Decorator/RTSHasLeftChaseRadius.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "GameFramework/Controller.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSHasLeftChaseRadius::URTSHasLeftChaseRadius(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Has Left Chase Radius";
}

bool URTSHasLeftChaseRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return false;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return false;
	}

	const URTSAttackComponent* AttackComponent = Pawn->FindComponentByClass<URTSAttackComponent>();
	if (!IsValid(AttackComponent))
	{
		return false;
	}

	return AttackComponent->GetChaseRadius() < (Pawn->GetActorLocation() - OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName)).Length();
}

FString URTSHasLeftChaseRadius::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Home Location", BlackboardKey.SelectedKeyName.ToString()}}, GetFlowAbortMode(), IsInversed());
}
