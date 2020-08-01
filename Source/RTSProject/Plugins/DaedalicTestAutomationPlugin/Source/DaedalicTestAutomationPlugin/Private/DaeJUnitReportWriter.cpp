#include "DaeJUnitReportWriter.h"

FString FDaeJUnitReportWriter::CreateReport(const FString& Name,
                                            const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    // Write a JUnit XML report based on FXmlFile::WriteNodeHierarchy.
    // Unfortunately, FXmlNode::Tag is private, so we have to do the hard work here ourselves...
    FString XmlString;

    XmlString += TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") LINE_TERMINATOR;
    XmlString += TEXT("<testsuite");
    XmlString += FString::Printf(TEXT(" name=\"%s\""), *Name);
    XmlString += FString::Printf(TEXT(" tests=\"%d\""), NumTotalTests(TestSuites));
    XmlString += FString::Printf(TEXT(" skipped=\"%d\""), NumSkippedTests(TestSuites));
    XmlString += FString::Printf(TEXT(" failures=\"%d\""), NumFailedTests(TestSuites));
    XmlString += FString::Printf(TEXT(" errors=\"0\""));
    XmlString += FString::Printf(TEXT(" time=\"%f\""), GetTotalTimeSeconds(TestSuites));
    XmlString += FString::Printf(TEXT(" timestamp=\"%s\""), *GetTimestamp(TestSuites));
    XmlString += TEXT(">") LINE_TERMINATOR;

    for (const FDaeTestSuiteResult& TestSuiteResult : TestSuites)
    {
        const FString& TestClassName = FString::Printf(TEXT("%s.%s"), *TestSuiteResult.MapName,
                                                       *TestSuiteResult.TestSuiteName);

        for (const FDaeTestResult& TestResult : TestSuiteResult.TestResults)
        {
            XmlString += TEXT("    <testcase");
            XmlString += FString::Printf(TEXT(" name=\"%s\""), *TestResult.TestName);
            XmlString += FString::Printf(TEXT(" classname=\"%s\""), *TestClassName);
            XmlString += FString::Printf(TEXT(" time=\"%f\""), TestResult.TimeSeconds);
            XmlString += TEXT(">") LINE_TERMINATOR;

            if (TestResult.HasFailed())
            {
                XmlString +=
                    FString::Printf(TEXT("        <failure type=\"Assertion failed\">%s</failure>"),
                                    *TestResult.FailureMessage)
                    + LINE_TERMINATOR;
            }
            else if (TestResult.WasSkipped())
            {
                XmlString +=
                    FString::Printf(TEXT("        <skipped>%s</failure>"), *TestResult.SkipReason)
                    + LINE_TERMINATOR;
            }

            XmlString += TEXT("    </testcase>") LINE_TERMINATOR;
        }
    }

    XmlString += TEXT("</testsuite>") LINE_TERMINATOR;

    return XmlString;
}

int32 FDaeJUnitReportWriter::NumTotalTests(const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    int32 TotalTests = 0;

    for (const FDaeTestSuiteResult& TestSuite : TestSuites)
    {
        TotalTests += TestSuite.NumTotalTests();
    }

    return TotalTests;
}

int32 FDaeJUnitReportWriter::NumFailedTests(const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    int32 FailedTests = 0;

    for (const FDaeTestSuiteResult& TestSuite : TestSuites)
    {
        FailedTests += TestSuite.NumFailedTests();
    }

    return FailedTests;
}

int32 FDaeJUnitReportWriter::NumSkippedTests(const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    int32 SkippedTests = 0;

    for (const FDaeTestSuiteResult& TestSuite : TestSuites)
    {
        SkippedTests += TestSuite.NumSkippedTests();
    }

    return SkippedTests;
}

float FDaeJUnitReportWriter::GetTotalTimeSeconds(const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    float TimeSeconds = 0;

    for (const FDaeTestSuiteResult& TestSuite : TestSuites)
    {
        TimeSeconds += TestSuite.GetTotalTimeSeconds();
    }

    return TimeSeconds;
}

FString FDaeJUnitReportWriter::GetTimestamp(const TArray<FDaeTestSuiteResult>& TestSuites) const
{
    FDateTime Timestamp = TestSuites.Num() > 0 ? TestSuites[0].Timestamp : FDateTime::UtcNow();
    return Timestamp.ToIso8601();
}
