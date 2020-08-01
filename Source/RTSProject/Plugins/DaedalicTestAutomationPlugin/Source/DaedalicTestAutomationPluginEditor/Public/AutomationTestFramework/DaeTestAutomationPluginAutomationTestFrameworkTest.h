#pragma once

#include <CoreMinimal.h>
#include <Misc/AutomationTest.h>

/** Single test to be registered with the Unreal Automation Test Framework. Implementation based on IMPLEMENT_SIMPLE_AUTOMATION_TEST_PRIVATE macro. */
class FDaeTestAutomationPluginAutomationTestFrameworkTest : FAutomationTestBase
{
public:
    FDaeTestAutomationPluginAutomationTestFrameworkTest(const FString& InMapName);

    virtual uint32 GetTestFlags() const override;
    virtual uint32 GetRequiredDeviceNum() const override;
    virtual FString GetTestSourceFileName() const override;
    virtual int32 GetTestSourceFileLine() const override;

    FString GetMapName() const;

protected:
    virtual void GetTests(TArray<FString>& OutBeautifiedNames,
                          TArray<FString>& OutTestCommands) const override;
    virtual bool RunTest(const FString& Parameters) override;
    virtual FString GetBeautifiedTestName() const override;

private:
    FString MapName;
};
