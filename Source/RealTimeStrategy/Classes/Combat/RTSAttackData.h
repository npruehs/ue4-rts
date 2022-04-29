#pragma once

#include "CoreMinimal.h"

#include "GameFramework/DamageType.h"
#include "Templates/SubclassOf.h"

#include "Combat/RTSProjectile.h"

#include "RTSAttackData.generated.h"


USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSAttackData
{
	GENERATED_BODY()

public:
	/** Time before this attack can be used again, in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Cooldown;

	/** Damage dealt by this attack. Negative values could mean healing, depending on the game. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Damage;

	/** Type of the damage caused by this attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TSubclassOf<UDamageType> DamageType;

	/** Range of this attack, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float Range;

	/** Type of the projectile to spawn. If not set, damage will be dealt instantly. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TSubclassOf<ARTSProjectile> ProjectileClass;
};
