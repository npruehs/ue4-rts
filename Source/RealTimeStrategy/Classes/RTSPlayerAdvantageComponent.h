#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSPlayerAdvantageComponent.generated.h"


/**
* Attach to player or AI controllers. Provides bonuses for various gameplay elements.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSPlayerAdvantageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSPlayerAdvantageComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Whether units controlled by the player are invulnerable, or not. */
	bool IsGodModeEnabled() const;

	/** Gets the factor to multiply all construction and production speed with. */
	float GetSpeedBoostFactor() const;

	/** Gets the factor to multiply all damage dealt by actors owned by the player with. */
	float GetOutgoingDamageFactor() const;

	/** Sets whether units controlled by the player are invulnerable, or not. */
	void SetGodModeEnabled(bool bInGodModeEnabled);

	/** Sets the factor to multiply all construction and production speed with. */
	void SetSpeedBoostFactor(float InSpeedBoostFactor);

	/** Sets the factor to multiply all damage dealt by actors owned by the player with. */
	void SetOutgoingDamageFactor(float InOutgoingDamageFactor);

private:
	/** Whether units controlled by the player are invulnerable, or not. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Advantage")
	bool bGodModeEnabled;

	/** Factor to multiply all construction and production speed with. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Advantage")
	float SpeedBoostFactor;

	/** Factor to multiply all damage dealt by actors owned by the player with. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS|Advantage")
	float OutgoingDamageFactor;
};
