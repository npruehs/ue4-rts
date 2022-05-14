// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Decorator/RTSIsConstructionFinishedDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSIsConstructionFinishedDecorator::URTSIsConstructionFinishedDecorator(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is Construction Finished";
}

bool URTSIsConstructionFinishedDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (!IsValid(Actor))
	{
		return false;
	}

	const URTSConstructionSiteComponent* ConstructionSiteComponent = Actor->FindComponentByClass<URTSConstructionSiteComponent>();
	if (!IsValid(ConstructionSiteComponent))
	{
		return false;
	}

	return ConstructionSiteComponent->IsFinished();
}

FString URTSIsConstructionFinishedDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Target", BlackboardKey.SelectedKeyName.ToString()}}, GetFlowAbortMode(), IsInversed());
}
