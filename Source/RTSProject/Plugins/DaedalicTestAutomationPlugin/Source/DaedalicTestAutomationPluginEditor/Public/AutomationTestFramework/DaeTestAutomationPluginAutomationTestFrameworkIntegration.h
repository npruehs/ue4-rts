#pragma once

#include "DaeTestAutomationPluginAutomationTestFrameworkTest.h"
#include <CoreMinimal.h>

/** Integration with the Unreal Automation Test Framework. Discovers tests based on the plugin settings. */
class FDaeTestAutomationPluginAutomationTestFrameworkIntegration
{
public:
    /** Sets the path to look for test maps in, re-discovering tests afterwards. */
    void SetTestMapPath(const FString& InTestMapPath);

private:
    /** Path to look for test maps in. */
    FString TestMapPath;

    /** Currently registered automation tests. */
    TArray<TSharedPtr<FDaeTestAutomationPluginAutomationTestFrameworkTest>> Tests;
};
