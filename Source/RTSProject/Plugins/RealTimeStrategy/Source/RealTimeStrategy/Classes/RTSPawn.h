#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Pawn.h"

#include "RTSPawn.generated.h"


class AController;


/**
* Pawn with RTS features, such as taking damage.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSPawn : public APawn
{
    GENERATED_BODY()

public:
    ARTSPawn();

    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
