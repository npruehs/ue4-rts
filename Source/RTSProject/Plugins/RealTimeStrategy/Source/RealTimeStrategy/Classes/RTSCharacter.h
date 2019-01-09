#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Character.h"

#include "RTSCharacter.generated.h"


class AController;


/**
* Character with RTS features, such as taking damage.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
