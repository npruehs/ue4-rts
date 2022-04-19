#include "Vision/RTSVisibleActor.h"

#include "GameFramework/Actor.h"

#include "RTSOwnerComponent.h"
#include "Vision/RTSVisibleComponent.h"
#include "Vision/RTSVisionComponent.h"


FRTSVisibleActor::FRTSVisibleActor()
	: Actor(nullptr),
	  OwnerComponent(nullptr),
	  VisibleComponent(nullptr),
	  VisionComponent(nullptr)
{
}

FRTSVisibleActor::FRTSVisibleActor(AActor* InActor)
	: Actor(InActor)
{
	OwnerComponent = InActor->FindComponentByClass<URTSOwnerComponent>();
	VisibleComponent = InActor->FindComponentByClass<URTSVisibleComponent>();
	VisionComponent = InActor->FindComponentByClass<URTSVisionComponent>();
}

bool FRTSVisibleActor::IsActorValid() const
{
	return IsValid(Actor) && IsValid(OwnerComponent) && IsValid(VisibleComponent);
}
