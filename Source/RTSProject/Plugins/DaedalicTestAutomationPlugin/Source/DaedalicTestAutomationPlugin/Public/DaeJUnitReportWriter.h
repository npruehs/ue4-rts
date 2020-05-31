#pragma once

#include <CoreMinimal.h>

#include "DaeTestSuiteResult.h"

/** Writes test reports based on the Apache Ant JUnit report format (based on org.junit.platform.reporting.legacy.xml.XmlReportWriter.writeTestsuite). */
class DAEDALICTESTAUTOMATIONPLUGIN_API FDaeJUnitReportWriter
{
public:
    /** Returns an XML report based on the Apache Ant JUnit report format. */
    FString CreateReport(const FString& Name, const TArray<FDaeTestSuiteResult>& TestSuites) const;

private:
    int32 NumTotalTests(const TArray<FDaeTestSuiteResult>& TestSuites) const;
    int32 NumFailedTests(const TArray<FDaeTestSuiteResult>& TestSuites) const;
    int32 NumSkippedTests(const TArray<FDaeTestSuiteResult>& TestSuites) const;
    float GetTotalTimeSeconds(const TArray<FDaeTestSuiteResult>& TestSuites) const;
    FString GetTimestamp(const TArray<FDaeTestSuiteResult>& TestSuites) const;
};
