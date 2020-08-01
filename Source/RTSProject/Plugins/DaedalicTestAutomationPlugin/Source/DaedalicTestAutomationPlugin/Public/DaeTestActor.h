#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "DaeTestActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDaeTestActorTestSuccessfulSignature, ADaeTestActor*,
                                             Test, UObject*, Parameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDaeTestActorTestFailedSignature, ADaeTestActor*,
                                               Test, UObject*, Parameter, const FString&,
                                               FailureMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDaeTestActorTestSkippedSignature, ADaeTestActor*,
                                               Test, UObject*, Parameter, const FString&,
                                               SkipReason);

class ADaeTestParameterProviderActor;

/** Single automated test to be run as part of a test suite. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API ADaeTestActor : public AActor
{
    GENERATED_BODY()

public:
    ADaeTestActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Applies additional providers for appending parameters for this test. */
    void ApplyParameterProviders();

    /** Starts executing this test. */
    void RunTest(UObject* TestParameter);

    /** Finishes execution of this test, automatically following up with the Assert step. */
    UFUNCTION(BlueprintCallable)
    void FinishAct();

    /** Gets how long this test is allowed to run before it fails automatically, in seconds. */
    float GetTimeoutInSeconds() const;

    /** Gets the parameters to run this test with, one per run.  */
    TArray<TSoftObjectPtr<UObject>> GetParameters() const;

    /** Gets the parameter for the current test run. */
    UFUNCTION(BlueprintPure)
    UObject* GetCurrentParameter() const;

    /** Event when this test has finished successfully. */
    virtual void NotifyOnTestSuccessful();

    /** Event when this test has failed. */
    virtual void NotifyOnTestFailed(const FString& Message);

    /** Event when this test has been skipped. */
    virtual void NotifyOnTestSkipped(const FString& InSkipReason);

    /** Event when this test should verify its preconditions. */
    virtual void NotifyOnAssume(UObject* Parameter);

    /** Event when this test should set up. */
    virtual void NotifyOnArrange(UObject* Parameter);

    /** Event when this test should execute. */
    virtual void NotifyOnAct(UObject* Parameter);

    /** Event when this test should check the results. */
    virtual void NotifyOnAssert(UObject* Parameter);

    /** Event when this test should verify its preconditions. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Assume"))
    void ReceiveOnAssume(UObject* Parameter);

    /** Event when this test should set up. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Arrange"))
    void ReceiveOnArrange(UObject* Parameter);

    /** Event when this test should execute. This is a latent event: You need to call FinishAct when you're finished. */
    UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Act"))
    void ReceiveOnAct(UObject* Parameter);

    /** Event when this test should check the results. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Assert"))
    void ReceiveOnAssert(UObject* Parameter);

    /** Event when this test has finished successfully. */
    FDaeTestActorTestSuccessfulSignature OnTestSuccessful;

    /** Event when this test has failed. */
    FDaeTestActorTestFailedSignature OnTestFailed;

    /** Event when this test has been skipped. */
    FDaeTestActorTestSkippedSignature OnTestSkipped;

private:
    /** How long this test is allowed to run before it fails automatically, in seconds. */
    UPROPERTY(EditAnywhere)
    float TimeoutInSeconds;

    /** Reason for skipping this test. Test will be skipped if this is not empty. Useful for temporarily disabling unstable tests. */
    UPROPERTY(EditAnywhere)
    FString SkipReason;

    /** Parameterizes this test, running it multiple times, once per specified parameter.  */
    UPROPERTY(EditAnywhere)
    TArray<TSoftObjectPtr<UObject>> Parameters;

    /** Additional providers for appending parameters for this test. Applied exactly once before the first test run. */
    UPROPERTY(EditAnywhere)
    TArray<ADaeTestParameterProviderActor*> ParameterProviders;

    /** Parameter for the current test run. */
    UPROPERTY()
    UObject* CurrentParameter;

    /** Whether this test has finished executing (either with success or failure). */
    bool bHasResult;
};
