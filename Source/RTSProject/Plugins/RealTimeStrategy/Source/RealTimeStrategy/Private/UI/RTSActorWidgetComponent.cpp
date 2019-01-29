#include "UI/RTSActorWidgetComponent.h"


URTSActorWidgetComponent::URTSActorWidgetComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    // Set reasonable default values.
    Space = EWidgetSpace::Screen;
}
