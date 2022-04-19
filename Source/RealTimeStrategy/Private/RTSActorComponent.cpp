#include "RTSActorComponent.h"

void URTSActorComponent::AddGameplayTags(FGameplayTagContainer& InOutTagContainer)
{
	InOutTagContainer.AppendTags(InitialGameplayTags);
}
