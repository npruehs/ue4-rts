#include "Vision/RTSVisionActor.h"

#include "GameFramework/Actor.h"

#include "Vision/RTSVisionComponent.h"


FRTSVisionActor::FRTSVisionActor()
    : Actor(nullptr),
    VisionComponent(nullptr)
{
}

FRTSVisionActor::FRTSVisionActor(AActor* InActor)
    : Actor(InActor)
{
    VisionComponent = InActor->FindComponentByClass<URTSVisionComponent>();
}

bool FRTSVisionActor::IsActorValid() const
{
    return IsValid(Actor) && IsValid(VisionComponent);
}
