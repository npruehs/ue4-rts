#include "Production/RTSProductionProgressBarWidgetComponent.h"

#include "GameFramework/Actor.h"

#include "Production/RTSProductionComponent.h"

void URTSProductionProgressBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();

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

void URTSProductionProgressBarWidgetComponent::OnProductionProgressChanged(AActor* Actor, int32 QueueIndex, float ProgressPercentage)
{
	UpdateProductionProgressBar(QueueIndex, ProgressPercentage);
}
