// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSIsInRangeDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSIsInRangeDecorator::URTSIsInRangeDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Is In Range";
}

bool URTSIsInRangeDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	AActor* SelfActor = Controller->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName));

	if (!IsValid(SelfActor) || !IsValid(TargetActor))
	{
		return false;
	}

	const float AllowedDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(Range.SelectedKeyName) + AcceptanceRadius;
	return URTSCollisionLibrary::GetActorDistance(SelfActor, TargetActor, true) <= AllowedDistance;
}

FString URTSIsInRangeDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {
		{"Target", Target.SelectedKeyName.ToString()},
		{"Range", Range.SelectedKeyName.ToString()},
		{"AcceptanceRadius", FString::SanitizeFloat(AcceptanceRadius)},
	}, GetFlowAbortMode(), IsInversed());
}
