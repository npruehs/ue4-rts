// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSHasGameplayTagDecorator.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Libraries/RTSFunctionLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"


URTSHasGameplayTagDecorator::URTSHasGameplayTagDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Has GameplayTag";
}

bool URTSHasGameplayTagDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return false;
	}

	return URTSGameplayTagLibrary::HasGameplayTag(Cast<AActor>(Controller->GetPawn()), Tag);
}

FString URTSHasGameplayTagDecorator::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Tag", Tag.GetTagName().ToString()}}, GetFlowAbortMode(), IsInversed());
}
