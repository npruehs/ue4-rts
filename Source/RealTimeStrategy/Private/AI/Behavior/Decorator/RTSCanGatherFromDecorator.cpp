// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSCanGatherFromDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "GameFramework/Controller.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSCanGatherFromDecorator::URTSCanGatherFromDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Can Gather From";
}

bool URTSCanGatherFromDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (!IsValid(Target))
	{
		return false;
	}

	return GathererComponent->CanGatherFrom(Target);
}

FString URTSCanGatherFromDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Resource", BlackboardKey.SelectedKeyName.ToString()}}, GetFlowAbortMode(), IsInversed());
}
