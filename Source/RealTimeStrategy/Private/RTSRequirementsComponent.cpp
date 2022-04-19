#include "RTSRequirementsComponent.h"

TArray<TSubclassOf<AActor>> URTSRequirementsComponent::GetRequiredActors() const
{
	return RequiredActors;
}
