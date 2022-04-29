#include "RTSDescriptionComponent.h"

FText URTSDescriptionComponent::GetDescription() const
{
	return Description;
}

void URTSDescriptionComponent::SetDescription(const FText& NewDescription)
{
	Description = NewDescription;
}
