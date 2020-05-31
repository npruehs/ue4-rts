#pragma once

#include "DaeTestAutomationPluginAutomationTestFrameworkTestContext.h"
#include <CoreMinimal.h>
#include <Misc/AutomationTest.h>

class ADaeTestSuiteActor;

/** Waits for the current test suite to finish. */
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(
    FDaeTestAutomationPluginWaitForEndOfTestSuite,
    FDaeTestAutomationPluginAutomationTestFrameworkTestContext, Context);
