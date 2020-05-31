#pragma once

#include <CoreMinimal.h>

class ADaeTestSuiteActor;

/** Context to run a single test for the Unreal Automation Test Framework within. */
class FDaeTestAutomationPluginAutomationTestFrameworkTestContext
{
public:
    ADaeTestSuiteActor* CurrentTestSuite;

    FDaeTestAutomationPluginAutomationTestFrameworkTestContext();
};
