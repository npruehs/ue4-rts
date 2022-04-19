#include "Combat/RTSHealthBarWidgetComponent.h"

#include "RTSLog.h"
#include "Combat/RTSCombatComponent.h"
#include "GameFramework/Actor.h"

#include "Combat/RTSHealthComponent.h"


void URTSHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	CombatComponent = Owner->FindComponentByClass<URTSCombatComponent>();
	HealthComponent = Owner->FindComponentByClass<URTSHealthComponent>();

	if (CombatComponent && HealthComponent)
	{
		UE_LOG(LogRTS, Warning, TEXT("Using both HealhComponent and CombatComponent is not expected. The health will only obtained from the CombatComponent. Actor: %s"), *Owner->GetActorLabel())
	}
}

void URTSHealthBarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(CombatComponent))
	{
		UpdateHealthBar(CombatComponent->GetCurrentHealth() / CombatComponent->GetMaximumHealth());
	}
	else if (IsValid(HealthComponent))
	{
		UpdateHealthBar(HealthComponent->GetCurrentHealth() / HealthComponent->GetMaximumHealth());
	}
}
