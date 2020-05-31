#include "DaeTestSuiteResult.h"

int32 FDaeTestSuiteResult::NumTotalTests() const
{
    return TestResults.Num();
}

int32 FDaeTestSuiteResult::NumSuccessfulTests() const
{
    return NumTotalTests() - NumFailedTests() - NumSkippedTests();
}

int32 FDaeTestSuiteResult::NumFailedTests() const
{
    int32 FailedTests = 0;

    for (const FDaeTestResult& TestResult : TestResults)
    {
        if (TestResult.HasFailed())
        {
            ++FailedTests;
        }
    }

    return FailedTests;
}

int32 FDaeTestSuiteResult::NumSkippedTests() const
{
    int32 SkippedTests = 0;

    for (const FDaeTestResult& TestResult : TestResults)
    {
        if (TestResult.WasSkipped())
        {
            ++SkippedTests;
        }
    }

    return SkippedTests;
}

float FDaeTestSuiteResult::GetTotalTimeSeconds() const
{
    float TimeSeconds = 0.0f;

    for (const FDaeTestResult& TestResult : TestResults)
    {
        TimeSeconds += TestResult.TimeSeconds;
    }

    return TimeSeconds;
}
