#include "Orders/RTSOrder.h"

#include "RTSPawnAIController.h"
#include "GameFramework/Pawn.h"
#include "Orders/RTSOrderData.h"

bool URTSOrder::CanObeyOrder(const AActor* OrderedActor, int32 Index) const
{
	return true;
}

bool URTSOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	return true;
}

void URTSOrder::AddOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	IssueOrder(OrderedActor, TargetData, Index, true);
}

void URTSOrder::SetOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	IssueOrder(OrderedActor, TargetData, Index, false);
}

void URTSOrder::IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index, bool bAdd) const
{
	const APawn* Pawn = Cast<APawn>(OrderedActor);

	if (!IsValid(Pawn))
	{
		return;
	}

	ARTSPawnAIController* PawnController = Cast<ARTSPawnAIController>(Pawn->GetController());

	if (!IsValid(PawnController))
	{
		return;
	}

	// Issue order.
	FRTSOrderData Order;
	Order.OrderClass = GetClass();
	Order.TargetActor = TargetData.Actor;
	Order.TargetLocation = TargetData.Location;
	Order.Index = Index;

	if (bAdd)
	{
		PawnController->AddOrder(Order);
	}
	else
	{
		PawnController->IssueOrder(Order);
	}
}

ERTSOrderTargetType URTSOrder::GetTargetType() const
{
	return TargetType;
}

ERTSOrderGroupExecutionType URTSOrder::GetGroupExecutionType() const
{
	return GroupExecutionType;
}

FRTSOrderTagRequirements URTSOrder::GetIssueTagRequirements() const
{
	return IssueTagRequirements;
}

UTexture2D* URTSOrder::GetNormalIcon() const
{
	return NormalIcon;
}

UTexture2D* URTSOrder::GetHoveredIcon() const
{
	return HoveredIcon;
}

UTexture2D* URTSOrder::GetPressedIcon() const
{
	return PressedIcon;
}

UTexture2D* URTSOrder::GetDisabledIcon() const
{
	return DisabledIcon;
}

FText URTSOrder::GetName() const
{
	return Name;
}

FText URTSOrder::GetDescription(const AActor* OrderedActor, int32 Index) const
{
	return Description;
}
