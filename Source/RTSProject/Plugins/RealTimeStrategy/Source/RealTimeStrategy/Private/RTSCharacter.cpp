#include "RealTimeStrategyPCH.h"
#include "RTSCharacter.h"

#include "Combat/RTSHealthComponent.h"


ARTSCharacter::ARTSCharacter()
{
	// Enable replication.
	bReplicates = true;
}

float ARTSCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Adjust health.
	URTSHealthComponent* HealthComponent = FindComponentByClass<URTSHealthComponent>();
	if (!HealthComponent)
	{
		return 0.0f;
	}

	return HealthComponent->TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}
