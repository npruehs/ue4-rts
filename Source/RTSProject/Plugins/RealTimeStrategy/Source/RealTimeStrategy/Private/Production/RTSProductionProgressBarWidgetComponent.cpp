#include "Production/RTSProductionProgressBarWidgetComponent.h"

#include "GameFramework/Actor.h"

#include "Production/RTSProductionComponent.h"

void URTSProductionProgressBarWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    URTSProductionComponent* ProductionComponent = Owner->FindComponentByClass<URTSProductionComponent>();

    if (!IsValid(ProductionComponent))
    {
        return;
    }

    ProductionComponent->OnProductionProgressChanged.AddDynamic(this,
        &URTSProductionProgressBarWidgetComponent::OnProductionProgressChanged);
}

void URTSProductionProgressBarWidgetComponent::OnProductionProgressChanged(int32 QueueIndex, float ProgressPercentage)
{
    UpdateProductionProgressBar(QueueIndex, ProgressPercentage);
}
