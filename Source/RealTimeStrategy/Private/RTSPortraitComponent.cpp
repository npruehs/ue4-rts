#include "RTSPortraitComponent.h"

#include "Engine/Texture2D.h"


UTexture2D* URTSPortraitComponent::GetPortrait() const
{
	return Portrait;
}

void URTSPortraitComponent::SetPortrait(UTexture2D* NewPortrait)
{
	Portrait = NewPortrait;
}
