// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSResourceIsReadyDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Economy/RTSResourceSourceComponent.h"
#include "GameFramework/Controller.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSResourceIsReadyDecorator::URTSResourceIsReadyDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Is Resource Ready";
}

bool URTSResourceIsReadyDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return false;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return false;
	}

	const AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (!IsValid(Target))
	{
		return false;
	}

	const URTSResourceSourceComponent* ResourceSourceComponent = Target->FindComponentByClass<URTSResourceSourceComponent>();

	if (!IsValid(ResourceSourceComponent))
	{
		return false;
	}

	return !ResourceSourceComponent->MustGathererEnter() || ResourceSourceComponent->CanGathererEnter(Pawn);
}

FString URTSResourceIsReadyDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Resource", BlackboardKey.SelectedKeyName.ToString()}}, GetFlowAbortMode(), IsInversed());
}
