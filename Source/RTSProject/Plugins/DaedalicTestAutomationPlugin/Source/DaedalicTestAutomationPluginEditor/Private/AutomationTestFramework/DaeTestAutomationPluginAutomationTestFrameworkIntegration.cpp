#include "AutomationTestFramework/DaeTestAutomationPluginAutomationTestFrameworkIntegration.h"
#include "DaeTestEditorLogCategory.h"
#include <FileHelpers.h>
#include <Misc/PackageName.h>
#include <Misc/Paths.h>

void FDaeTestAutomationPluginAutomationTestFrameworkIntegration::SetTestMapPath(
    const FString& InTestMapPath)
{
    UE_LOG(LogDaeTestEditor, Log, TEXT("Discovering tests from: %s"), *InTestMapPath);

    // Unregister existing tests.
    Tests.Empty();

    // Register new tests (based on FLoadAllMapsInEditorTest).
    TArray<FString> PackageFiles;
    FEditorFileUtils::FindAllPackageFiles(PackageFiles);

    // Iterate over all files, adding the ones with the map extension.
    const FString PatternToCheck = FString::Printf(TEXT("/%s/"), *InTestMapPath);

    for (const FString& Filename : PackageFiles)
    {
        if (FPaths::GetExtension(Filename, true) == FPackageName::GetMapPackageExtension()
            && Filename.Contains(*PatternToCheck))
        {
            TSharedPtr<FDaeTestAutomationPluginAutomationTestFrameworkTest> NewTest =
                MakeShareable(new FDaeTestAutomationPluginAutomationTestFrameworkTest(Filename));
            Tests.Add(NewTest);

            UE_LOG(LogDaeTestEditor, Log, TEXT("Discovered test: %s"), *NewTest->GetMapName());
        }
    }
}
