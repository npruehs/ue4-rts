#include "DaeTestTriggerBox.h"
#include "DaeTestLogCategory.h"

void ADaeTestTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    bWasTriggered = false;

    OnActorBeginOverlap.AddDynamic(this, &ADaeTestTriggerBox::OnActorBeginOverlapBroadcast);
}

bool ADaeTestTriggerBox::WasTriggered() const
{
    return bWasTriggered;
}

void ADaeTestTriggerBox::OnActorBeginOverlapBroadcast(AActor* OverlappedActor, AActor* OtherActor)
{
    bWasTriggered = true;

    UE_LOG(LogDaeTest, Log, TEXT("%s was triggered by %s."), *GetName(), *OtherActor->GetName());
}
