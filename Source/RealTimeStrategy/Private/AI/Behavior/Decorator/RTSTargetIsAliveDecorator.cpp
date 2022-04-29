// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSTargetIsAliveDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSTargetIsAliveDecorator::URTSTargetIsAliveDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Is Alive";
}

bool URTSTargetIsAliveDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!IsValid(OwnerComp.GetBlackboardComponent()))
	{
		return false;
	}

	const FName SelectedBlackboardKey = GetSelectedBlackboardKey();

	const UObject* AsObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelectedBlackboardKey);

	return IsValid(AsObject);
}

FString URTSTargetIsAliveDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Target", BlackboardKey.SelectedKeyName.ToString()}}, GetFlowAbortMode(), IsInversed());
}
