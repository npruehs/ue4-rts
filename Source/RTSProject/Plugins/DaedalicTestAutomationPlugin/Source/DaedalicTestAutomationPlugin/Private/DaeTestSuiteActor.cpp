#include "DaeTestSuiteActor.h"
#include "DaeTestActor.h"
#include "DaeTestLogCategory.h"
#include <Engine/World.h>

ADaeTestSuiteActor::ADaeTestSuiteActor(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
    bRunInPIE = true;
    TestIndex = -1;

    PrimaryActorTick.bCanEverTick = true;

    // We need to be able to time out even while gameplay is paused.
    PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ADaeTestSuiteActor::BeginPlay()
{
    Super::BeginPlay();

    // Setup result data.
    Result.MapName = GetWorld()->GetMapName();
    Result.TestSuiteName = GetName();
    Result.Timestamp = FDateTime::UtcNow();
}

void ADaeTestSuiteActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!IsRunning())
    {
        // Check if we should run all tests immediately.
        // Happening in first Tick to make sure all actors have begun play.
        if (bRunInPIE && GetWorld()->IsPlayInEditor() && TestIndex < 0)
        {
            RunAllTests();
        }

        return;
    }

    TestTimeSeconds += DeltaSeconds;

    ADaeTestActor* CurrentTest = GetCurrentTest();

    if (TestTimeSeconds >= CurrentTest->GetTimeoutInSeconds())
    {
        // Enough waiting. Let's see the results.
        UE_LOG(LogDaeTest, Warning, TEXT("Timed out after %f seconds"),
               CurrentTest->GetTimeoutInSeconds());

        CurrentTest->FinishAct();
    }
}

void ADaeTestSuiteActor::RunAllTests()
{
    UE_LOG(LogDaeTest, Display, TEXT("ADaeTestSuiteActor::RunAllTests - Test Suite: %s"),
           *GetName());

    NotifyOnBeforeAll();

    TestIndex = -1;
    TestParameterIndex = -1;

    RunNextTest();
}

bool ADaeTestSuiteActor::IsRunning() const
{
    return IsValid(GetCurrentTest());
}

ADaeTestActor* ADaeTestSuiteActor::GetCurrentTest() const
{
    return Tests.IsValidIndex(TestIndex) ? Tests[TestIndex] : nullptr;
}

UObject* ADaeTestSuiteActor::GetCurrentTestParameter() const
{
    ADaeTestActor* Test = GetCurrentTest();

    if (!IsValid(Test))
    {
        return nullptr;
    }

    TArray<TSoftObjectPtr<UObject>> TestParameters = Test->GetParameters();

    if (!TestParameters.IsValidIndex(TestParameterIndex))
    {
        return nullptr;
    }

    TSoftObjectPtr<UObject> Parameter = TestParameters[TestParameterIndex];

    if (!Parameter.IsValid())
    {
        return nullptr;
    }

    return Parameter.LoadSynchronous();
}

FString ADaeTestSuiteActor::GetCurrentTestName() const
{
    ADaeTestActor* Test = GetCurrentTest();

    if (!IsValid(Test))
    {
        return FString();
    }

    FString TestName = Test->GetName();

    UObject* Parameter = GetCurrentTestParameter();

    if (IsValid(Parameter))
    {
        TestName += TEXT(" - ") + Parameter->GetName();
    }

    return TestName;
}

FDaeTestSuiteResult ADaeTestSuiteActor::GetResult() const
{
    return Result;
}

void ADaeTestSuiteActor::NotifyOnBeforeAll()
{
    ReceiveOnBeforeAll();
}

void ADaeTestSuiteActor::NotifyOnAfterAll()
{
    ReceiveOnAfterAll();
}

void ADaeTestSuiteActor::NotifyOnBeforeEach()
{
    ReceiveOnBeforeEach();
}

void ADaeTestSuiteActor::NotifyOnAfterEach()
{
    ReceiveOnAfterEach();
}

void ADaeTestSuiteActor::RunNextTest()
{
    ADaeTestActor* CurrentTest = GetCurrentTest();

    // Unregister events.
    if (IsValid(CurrentTest))
    {
        CurrentTest->OnTestSuccessful.RemoveDynamic(this, &ADaeTestSuiteActor::OnTestSuccessful);
        CurrentTest->OnTestFailed.RemoveDynamic(this, &ADaeTestSuiteActor::OnTestFailed);
        CurrentTest->OnTestSkipped.RemoveDynamic(this, &ADaeTestSuiteActor::OnTestSkipped);
    }

    // Prepare test run with next parameter.
    ++TestParameterIndex;

    UObject* CurrentTestParameter = GetCurrentTestParameter();

    if (!IsValid(CurrentTestParameter))
    {
        // Prepare next test.
        ++TestIndex;
        TestParameterIndex = 0;

        // Apply parameter providers.
        ADaeTestActor* NextTest = GetCurrentTest();

        if (IsValid(NextTest))
        {
            NextTest->ApplyParameterProviders();
        }
    }

    TestTimeSeconds = 0.0f;

    if (!Tests.IsValidIndex(TestIndex))
    {
        // All tests finished.
        UE_LOG(LogDaeTest, Display, TEXT("ADaeTestSuiteActor::RunNextTest - All tests finished."));

        NotifyOnAfterAll();

        // Check if any test failed.
        for (const FDaeTestResult& TestResult : Result.TestResults)
        {
            if (!TestResult.FailureMessage.IsEmpty())
            {
                OnTestSuiteFailed.Broadcast(this);
                return;
            }
        }

        OnTestSuiteSuccessful.Broadcast(this);
        return;
    }

    ADaeTestActor* Test = GetCurrentTest();

    if (IsValid(Test))
    {
        FString TestName = GetCurrentTestName();
        UE_LOG(LogDaeTest, Display, TEXT("ADaeTestSuiteActor::RunNextTest - Test: %s"), *TestName);

        // Register events.
        Test->OnTestSuccessful.AddDynamic(this, &ADaeTestSuiteActor::OnTestSuccessful);
        Test->OnTestFailed.AddDynamic(this, &ADaeTestSuiteActor::OnTestFailed);
        Test->OnTestSkipped.AddDynamic(this, &ADaeTestSuiteActor::OnTestSkipped);

        // Run test.
        NotifyOnBeforeEach();

        UObject* TestParameter = GetCurrentTestParameter();
        Test->RunTest(TestParameter);
    }
    else
    {
        UE_LOG(LogDaeTest, Error,
               TEXT("ADaeTestSuiteActor::RunNextTest - %s has invalid test at index %i, skipping."),
               *GetName(), TestIndex);

        RunNextTest();
    }
}

void ADaeTestSuiteActor::OnTestSuccessful(ADaeTestActor* Test, UObject* Parameter)
{
    if (Test != GetCurrentTest())
    {
        // Prevent tests from reporting multiple results.
        return;
    }

    FString CurrentTestName = GetCurrentTestName();

    UE_LOG(LogDaeTest, Display, TEXT("ADaeTestSuiteActor::OnTestSuccessful - Test: %s"),
           *CurrentTestName);

    // Store result.
    FDaeTestResult TestResult(CurrentTestName, TestTimeSeconds);
    Result.TestResults.Add(TestResult);

    // Run next test.
    NotifyOnAfterEach();

    RunNextTest();
}

void ADaeTestSuiteActor::OnTestFailed(ADaeTestActor* Test, UObject* Parameter,
                                      const FString& FailureMessage)
{
    if (Test != GetCurrentTest())
    {
        // Prevent tests from reporting multiple results.
        return;
    }

    FString CurrentTestName = GetCurrentTestName();

    UE_LOG(LogDaeTest, Error,
           TEXT("ADaeTestSuiteActor::OnTestFailed - Test: %s, FailureMessage: %s"),
           *CurrentTestName, *FailureMessage);

    // Store result.
    FDaeTestResult TestResult(CurrentTestName, TestTimeSeconds);
    TestResult.FailureMessage = FailureMessage;
    Result.TestResults.Add(TestResult);

    // Run next test.
    NotifyOnAfterEach();

    RunNextTest();
}

void ADaeTestSuiteActor::OnTestSkipped(ADaeTestActor* Test, UObject* Parameter,
                                       const FString& SkipReason)
{
    if (Test != GetCurrentTest())
    {
        // Prevent tests from reporting multiple results.
        return;
    }

    FString CurrentTestName = GetCurrentTestName();

    UE_LOG(LogDaeTest, Display,
           TEXT("ADaeTestSuiteActor::OnTestSkipped - Test: %s, SkipReason: %s"), *CurrentTestName,
           *SkipReason);

    // Store result.
    FDaeTestResult TestResult(CurrentTestName, TestTimeSeconds);
    TestResult.SkipReason = SkipReason;
    Result.TestResults.Add(TestResult);

    // Run next test.
    RunNextTest();
}
