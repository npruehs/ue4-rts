#include "Orders/RTSReturnResourcesOrder.h"

#include "GameFramework/Actor.h"

#include "Economy/RTSGathererComponent.h"
#include "Economy/RTSResourceDrainComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTargetType.h"


URTSReturnResourcesOrder::URTSReturnResourcesOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;
	GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

	IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanGather());
	IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_CarryingResources());

	IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Immobilized());
	IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Constructing());

	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Relationship_SamePlayer());
	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_AcceptsReturnedResources());

	IssueTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_UnderConstruction());
}

bool URTSReturnResourcesOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
	const URTSGathererComponent* GathererComponent = OrderedActor->FindComponentByClass<URTSGathererComponent>();

	if (!IsValid(GathererComponent))
	{
		return false;
	}

	const URTSResourceDrainComponent* ResourceDrainComponent = TargetData.Actor->FindComponentByClass<URTSResourceDrainComponent>();

	if (!IsValid(ResourceDrainComponent))
	{
		return false;
	}

	if (!ResourceDrainComponent->GetResourceTypes().Contains(GathererComponent->GetCarriedResourceType()))
	{
		return false;
	}

	return true;
}
