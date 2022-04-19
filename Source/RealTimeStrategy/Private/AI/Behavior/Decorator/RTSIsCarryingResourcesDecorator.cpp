// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSIsCarryingResourcesDecorator.h"

#include "Economy/RTSGathererComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Libraries/RTSFunctionLibrary.h"


URTSIsCarryingResourcesDecorator::URTSIsCarryingResourcesDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Is Carrying Resources";
}

bool URTSIsCarryingResourcesDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>();
	if (!IsValid(GathererComponent))
	{
		return false;
	}

	return GathererComponent->IsCarryingResources();
}

FString URTSIsCarryingResourcesDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {}, GetFlowAbortMode(), IsInversed());
}
