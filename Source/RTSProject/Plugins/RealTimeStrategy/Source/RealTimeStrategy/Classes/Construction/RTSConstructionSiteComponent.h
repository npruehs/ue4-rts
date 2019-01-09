#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "Construction/RTSConstructionState.h"
#include "Economy/RTSPaymentType.h"

#include "RTSConstructionSiteComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSConstructionSiteComponentConstructionStartedSignature, float, TotalConstructionTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSConstructionSiteComponentConstructionFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSConstructionSiteComponentConstructionCanceledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSConstructionSiteComponentBuilderConsumedSignature, AActor*, Builder);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSConstructionSiteComponentConstructionCostRefundedSignature, TSubclassOf<URTSResourceType>, ResourceType, float, ResourceAmount);


/** Allows constructing the actor over time. */
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSConstructionSiteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Whether the specified builder can be assigned to this construction site. */
	UFUNCTION(BlueprintPure)
	virtual bool CanAssignBuilder(AActor* Builder) const;

	/** Gets the current construction progress [0..1]. */
	UFUNCTION(BlueprintPure)
	float GetProgressPercentage() const;

	/** Whether the construction timer is currently being ticked, or not. */
	UFUNCTION(BlueprintPure)
	bool IsConstructing() const;

	/** Whether the construction is finished and the actor ready to use. */
	UFUNCTION(BlueprintPure)
	bool IsFinished() const;

	/** Starts constructing the actor, setting the timer. */
	UFUNCTION(BlueprintCallable)
	virtual void StartConstruction();

	/** Finishes constructing the actor. */
	UFUNCTION(BlueprintCallable)
	virtual void FinishConstruction();

	/** Cancels constructing the actor, destroying it. */
	UFUNCTION(BlueprintCallable)
	virtual void CancelConstruction();


    /** When to pay resources for construction. */
    UFUNCTION(BlueprintPure)
    ERTSPaymentType GetConstructionCostType() const;

    /** Gets the resources to pay for constructing the actor. */
    UFUNCTION(BlueprintPure)
    TMap<TSubclassOf<URTSResourceType>, float> GetConstructionCosts() const;

    /** Gets the time for constructing the actor, in seconds. */
    UFUNCTION(BlueprintPure)
    float GetConstructionTime() const;

    /** Whether any builders working at this construction site are destroyed when finished. */
    UFUNCTION(BlueprintPure)
    bool ConsumesBuilders() const;

    /** Gets how many builders may be assigned to this construction site at the same time. */
    UFUNCTION(BlueprintPure)
    int32 GetMaxAssignedBuilders() const;

    /** Gets the factor to multiply all passed construction time with, independent of any currently assigned builders. */
    UFUNCTION(BlueprintPure)
    float GetProgressMadeAutomatically() const;

    /** Gets the factor to multiply all passed construction time with, multiplied with the number of currently assigned builders. */
    UFUNCTION(BlueprintPure)
    float GetProgressMadePerBuilder() const;

    /** Gets the resources to refund when canceling construction. */
    UFUNCTION(BlueprintPure)
    float GetRefundFactor() const;

    /** Whether to start construction immediately after spawn, or not. */
    UFUNCTION(BlueprintPure)
    bool DoesStartImmediately() const;

    /** Whether the construction timer is currently being ticked, or not. */
    UFUNCTION(BlueprintPure)
    ERTSConstructionState GetState() const;

    /** Gets the time before the actor is constructed, in seconds. */
    UFUNCTION(BlueprintPure)
    float GetRemainingConstructionTime() const;

    /** Gets the builders currently working at this construction site. */
    UFUNCTION(BlueprintPure)
    TArray<AActor*> GetAssignedBuilders() const;


	/** Event when the construction timer has been started. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionStartedSignature OnConstructionStarted;

	/** Event when the construction timer has been expired. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionFinishedSignature OnConstructionFinished;

	/** Event when the construction has been canceled and the construction site destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionCanceledSignature OnConstructionCanceled;

	/** Event when a builder working at this construction site has been destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentBuilderConsumedSignature OnBuilderConsumed;

	/** Event when any construction costs have been refunded. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionCostRefundedSignature OnConstructionCostRefunded;

private:
    /** When to pay resources for construction. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSPaymentType ConstructionCostType;

    /** Resources to pay for constructing the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TMap<TSubclassOf<URTSResourceType>, float> ConstructionCosts;

    /** Time for constructing the actor, in seconds. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float ConstructionTime;

    /** Whether any builders working at this construction site are destroyed when finished. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bConsumesBuilders;

    /** How many builders may be assigned to this construction site at the same time. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    int32 MaxAssignedBuilders;

    /** Factor to multiply all passed construction time with, independent of any currently assigned builders. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float ProgressMadeAutomatically;

    /** Factor to multiply all passed construction time with, multiplied with the number of currently assigned builders. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float ProgressMadePerBuilder;

    /** Resources to refund when canceling construction. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    float RefundFactor;

    /** Whether to start construction immediately after spawn, or not. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bStartImmediately;

	/** Whether the construction timer is currently being ticked, or not. */
    UPROPERTY(replicated)
	ERTSConstructionState State;

    /** Time before the actor is constructed, in seconds. */
    UPROPERTY(replicated)
    float RemainingConstructionTime;

    /** Builders currently working at this construction site. */
    UPROPERTY()
    TArray<AActor*> AssignedBuilders;
};
