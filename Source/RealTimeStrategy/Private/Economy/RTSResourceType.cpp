#include "Economy/RTSResourceType.h"

FText URTSResourceType::GetResourceName() const
{
	return ResourceName;
}

UTexture2D* URTSResourceType::GetResourceIcon() const
{
	return ResourceIcon;
}

FLinearColor URTSResourceType::GetResourceColor() const
{
	return ResourceColor;
}
