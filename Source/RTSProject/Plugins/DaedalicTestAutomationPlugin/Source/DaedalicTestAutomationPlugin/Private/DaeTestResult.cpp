#include "DaeTestResult.h"

FDaeTestResult::FDaeTestResult()
    : FDaeTestResult(FString(), 0.0f)
{
}

FDaeTestResult::FDaeTestResult(FString InTestName, float InTimeSeconds)
    : TestName(InTestName)
    , TimeSeconds(InTimeSeconds)
{
}

bool FDaeTestResult::WasSuccessful() const
{
    return !HasFailed() && !WasSkipped();
}

bool FDaeTestResult::HasFailed() const
{
    return !FailureMessage.IsEmpty();
}

bool FDaeTestResult::WasSkipped() const
{
    return !SkipReason.IsEmpty();
}
