#include "Construction/RTSConstructionProgressBarWidgetComponent.h"

#include "GameFramework/Actor.h"

#include "Construction/RTSConstructionSiteComponent.h"


void URTSConstructionProgressBarWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    URTSConstructionSiteComponent* ConstructionSiteComponent = Owner->FindComponentByClass<URTSConstructionSiteComponent>();

    if (!IsValid(ConstructionSiteComponent))
    {
        return;
    }

    ConstructionSiteComponent->OnConstructionProgressChanged.AddDynamic(this,
        &URTSConstructionProgressBarWidgetComponent::OnConstructionProgressChanged);
}

void URTSConstructionProgressBarWidgetComponent::OnConstructionProgressChanged(float ProgressPercentage)
{
    UpdateConstructionProgressBar(ProgressPercentage);
}
