#include "Orders/RTSContinueConstructionOrder.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTargetType.h"


URTSContinueConstructionOrder::URTSContinueConstructionOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;
	GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_Any;

	IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanConstruct());

	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanBeConstructed());
	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_UnderConstruction());
	IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Relationship_SamePlayer());

	SuccessTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_UnderConstruction());
}
