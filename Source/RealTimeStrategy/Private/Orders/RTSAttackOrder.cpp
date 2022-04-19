#include "Orders/RTSAttackOrder.h"

#include "Combat/RTSAttackComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"

URTSAttackOrder::URTSAttackOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;
    GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

    IssueTagRequirements.SourceRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanAttack());

    IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Unarmed());

    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanBeAttacked());
    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
    IssueTagRequirements.TargetRequiredTags.AddTag(URTSGameplayTagLibrary::Relationship_Visible());

    IssueTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Invulnerable());
    IssueTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Relationship_Friendly());

    SuccessTagRequirements.TargetBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
}
