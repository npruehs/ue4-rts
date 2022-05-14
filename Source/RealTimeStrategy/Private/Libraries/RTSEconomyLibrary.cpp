#include "Libraries/RTSEconomyLibrary.h"

FText URTSEconomyLibrary::GetResourceName(TSubclassOf<URTSResourceType> ResourceClass)
{
	return ResourceClass ? ResourceClass->GetDefaultObject<URTSResourceType>()->GetResourceName() : FText();
}

UTexture2D* URTSEconomyLibrary::GetResourceIcon(TSubclassOf<URTSResourceType> ResourceClass)
{
	return ResourceClass ? ResourceClass->GetDefaultObject<URTSResourceType>()->GetResourceIcon() : nullptr;
}

FLinearColor URTSEconomyLibrary::GetResourceColor(TSubclassOf<URTSResourceType> ResourceClass)
{
	return ResourceClass ? ResourceClass->GetDefaultObject<URTSResourceType>()->GetResourceColor() : FLinearColor();
}
