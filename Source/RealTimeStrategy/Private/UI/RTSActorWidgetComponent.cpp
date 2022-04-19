#include "UI/RTSActorWidgetComponent.h"


URTSActorWidgetComponent::URTSActorWidgetComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	Space = EWidgetSpace::World;
}

void URTSActorWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URTSActorWidgetComponent::UpdatePositionAndSize_Implementation(const FVector2D& ActorScreenSize)
{
}
