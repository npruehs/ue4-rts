#include "Orders/RTSStopOrder.h"

#include "Libraries/RTSGameplayTagLibrary.h"
#include "Orders/RTSOrderTargetType.h"


URTSStopOrder::URTSStopOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TargetType = ERTSOrderTargetType::ORDERTARGET_None;
	GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

	IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Constructing());
}
