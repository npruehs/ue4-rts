#include "RTSPluginPCH.h"
#include "RTSHealthComponent.h"

#include "Net/UnrealNetwork.h"



URTSHealthComponent::URTSHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicated(true);

	// Set reasonable default values.
	CurrentHealth = 100.0f;
	MaximumHealth = 100.0f;
}

void URTSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSHealthComponent, CurrentHealth);
}

float URTSHealthComponent::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OldHealth = CurrentHealth;
	CurrentHealth -= Damage;
	float NewHealth = CurrentHealth;

	UE_LOG(LogRTS, Log, TEXT("Actor %s has taken %f damage from %s, reducing health to %f."),
		*GetOwner()->GetName(),
		Damage,
		*DamageCauser->GetName(),
		CurrentHealth);

	// Notify listeners.
	OnHealthChanged.Broadcast(OldHealth, NewHealth, DamageCauser);

	// Check if we've just died.
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogRTS, Log, TEXT("Actor %s has been killed."), *GetOwner()->GetName());

		// Get owner before destruction.
		AController* OwningPlayer = Cast<AController>(GetOwner()->GetOwner());

        // Notify listeners.
        OnKilled.Broadcast(OwningPlayer, DamageCauser);

		// Destroy this actor.
		GetOwner()->Destroy();

		// Notify game mode.
		ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode != nullptr)
		{
			GameMode->NotifyOnActorKilled(GetOwner(), OwningPlayer);
		}
	}

	return Damage;
}
