// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Decorator/RTSIsAtConstructionSiteDecorator.h"
#include "Construction/RTSBuilderComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSIsAtConstructionSiteDecorator::URTSIsAtConstructionSiteDecorator(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is At Construction Site";
}

bool URTSIsAtConstructionSiteDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const URTSBuilderComponent* BuilderComponent= Pawn->FindComponentByClass<URTSBuilderComponent>();
	if (!IsValid(BuilderComponent))
	{
		return false;
	}

	return IsValid(BuilderComponent->GetAssignedConstructionSite());
}

FString URTSIsAtConstructionSiteDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {}, GetFlowAbortMode(), IsInversed());
}
