#include "RTSNameComponent.h"

FText URTSNameComponent::GetNameRTS() const
{
    return Name;
}

void URTSNameComponent::SetName(const FText& NewName)
{
    Name = NewName;
}
