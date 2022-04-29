// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Decorator/RTSCanMoveDecorator.h"

#include "GameFramework/Controller.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSCanMoveDecorator::URTSCanMoveDecorator(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Can Move";
}

bool URTSCanMoveDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const UMovementComponent* MovementComponent = Pawn->FindComponentByClass<UMovementComponent>();
	if (!IsValid(MovementComponent))
	{
		return false;
	}

	return MovementComponent->GetMaxSpeed() > 0;
}

FString URTSCanMoveDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {}, GetFlowAbortMode(), IsInversed());
}
