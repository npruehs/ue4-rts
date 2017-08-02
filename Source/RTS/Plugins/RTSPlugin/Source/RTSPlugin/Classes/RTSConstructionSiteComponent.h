#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSConstructionSiteComponent.generated.h"


/** Allows constructing the actor over time. */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSConstructionSiteComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Time for constructing the actor, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
		float ConstructionTime;

	/** Whether to start construction immediately after spawn, or not. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
		bool bStartImmediately;

	/** Time before the actor is constructed, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", replicated)
		float RemainingConstructionTime;


	URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Gets the current construction progress [0..1]. */
	UFUNCTION(BlueprintCallable)
	float GetProgressPercentage() const;

	/** Whether the construction timer is currently being ticked, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsConstructing() const;

private:
	/** Whether the construction timer is currently being ticked, or not. */
	bool bConstructing;
};
