// © Acinex Games 2021

#include "AI/Behavior/Decorator/RTSHasOrder.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Libraries/RTSFunctionLibrary.h"
#include "Orders/RTSOrder.h"

URTSHasOrder::URTSHasOrder(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Has Order";
	Order = URTSOrder::StaticClass();
}

bool URTSHasOrder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsClass(TEXT("OrderClass")) == Order;
}

FString URTSHasOrder::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {{"Order", Order ? Order->GetName() : "None"}}, GetFlowAbortMode(), IsInversed());
}
