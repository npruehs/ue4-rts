#include "RTSNameComponent.h"

FText URTSNameComponent::GetName() const
{
	return Name;
}

void URTSNameComponent::SetName(const FText& NewName)
{
	Name = NewName;
}
