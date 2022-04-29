#include "Orders/RTSBeginConstructionOrder.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTargetType.h"


URTSBeginConstructionOrder::URTSBeginConstructionOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TargetType = ERTSOrderTargetType::ORDERTARGET_Location;
	GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_Any;

	IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanConstruct());

	IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Immobilized());
	IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Constructing());
}
