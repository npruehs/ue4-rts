#pragma once

#include "DaeTestSuiteResult.h"
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "DaeTestSuiteActor.generated.h"

class ADaeTestActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDaeTestSuiteActorTestSuiteSuccessfulSignature,
                                            ADaeTestSuiteActor*, TestSuite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDaeTestSuiteActorTestSuiteFailedSignature,
                                            ADaeTestSuiteActor*, TestSuite);

/** Collection of automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API ADaeTestSuiteActor : public AActor
{
    GENERATED_BODY()

public:
    ADaeTestSuiteActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    /** Runs all tests of this suite, in order. */
    void RunAllTests();

    /** Whether this test suite is currently running. */
    bool IsRunning() const;

    /** Gets the test that is currently running. */
    ADaeTestActor* GetCurrentTest() const;

    /** Gets the parameter for the current test run. */
    UObject* GetCurrentTestParameter() const;

    /** Gets the name of the current test. */
    FString GetCurrentTestName() const;

    /** Results of the whole test suite. */
    FDaeTestSuiteResult GetResult() const;

    /** Event when this test suite should set up. */
    virtual void NotifyOnBeforeAll();

    /** Event when this test suite has finished all tests. */
    virtual void NotifyOnAfterAll();

    /** Event when this test suite should set up for the next test. */
    virtual void NotifyOnBeforeEach();

    /** Event when this test suite should has finished a test. */
    virtual void NotifyOnAfterEach();

    /** Event when this test suite should set up. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeforeAll"))
    void ReceiveOnBeforeAll();

    /** Event when this test suite has finished all tests. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AfterAll"))
    void ReceiveOnAfterAll();

    /** Event when this test suite should set up for the next test. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeforeEach"))
    void ReceiveOnBeforeEach();

    /** Event when this test suite should has finished a test. This is NOT a latent event. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AfterEach"))
    void ReceiveOnAfterEach();

    /** Event when this test suite has successfully finished all tests. */
    FDaeTestSuiteActorTestSuiteSuccessfulSignature OnTestSuiteSuccessful;

    /** Event when any tests of this test suite have failed. */
    FDaeTestSuiteActorTestSuiteFailedSignature OnTestSuiteFailed;

private:
    /** Tests to run in this level. */
    UPROPERTY(EditInstanceOnly)
    TArray<ADaeTestActor*> Tests;

    /** Whether to automatically run this test suite on BeginPlay in Play In Editor. */
    UPROPERTY(EditInstanceOnly)
    bool bRunInPIE;

    /** Index of the current test. */
    int32 TestIndex;

    /** Index of the current parameter the current test is run with. */
    int32 TestParameterIndex;

    /** Time the current test has been running, in seconds. */
    float TestTimeSeconds;

    /** Results of the whole test suite. */
    FDaeTestSuiteResult Result;

    /** Runs the next test in this test suite. */
    void RunNextTest();

    UFUNCTION()
    void OnTestSuccessful(ADaeTestActor* Test, UObject* Parameter);

    UFUNCTION()
    void OnTestFailed(ADaeTestActor* Test, UObject* Parameter, const FString& FailureMessage);

    UFUNCTION()
    void OnTestSkipped(ADaeTestActor* Test, UObject* Parameter, const FString& SkipReason);
};
