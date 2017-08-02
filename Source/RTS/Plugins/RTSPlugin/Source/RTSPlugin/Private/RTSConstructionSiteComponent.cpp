#include "RTSPluginPrivatePCH.h"
#include "RTSConstructionSiteComponent.h"


URTSConstructionSiteComponent::URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSConstructionSiteComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSConstructionSiteComponent, RemainingConstructionTime);
}

void URTSConstructionSiteComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	if (bStartImmediately)
	{
		RemainingConstructionTime = ConstructionTime;
		bConstructing = true;
	}
}

void URTSConstructionSiteComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bConstructing)
	{
		return;
	}

	if (RemainingConstructionTime <= 0)
	{
		return;
	}

	RemainingConstructionTime -= DeltaTime;

	if (RemainingConstructionTime <= 0)
	{
		RemainingConstructionTime = 0;
		bConstructing = false;
	}
}

float URTSConstructionSiteComponent::GetProgressPercentage() const
{
	return 1 - (RemainingConstructionTime / ConstructionTime);
}

bool URTSConstructionSiteComponent::IsConstructing() const
{
	return bConstructing;
}
