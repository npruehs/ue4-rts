#include "DaeGauntletTestController.h"
#include "Kismet/GameplayStatics.h"
#include "DaeGauntletStates.h"
#include "DaeJUnitReportWriter.h"
#include "DaeTestAutomationPluginSettings.h"
#include "DaeTestLogCategory.h"
#include "DaeTestSuiteActor.h"
#include <AssetRegistryModule.h>
#include <EngineUtils.h>
#include <Engine/AssetManager.h>
#include <Misc/FileHelper.h>

void UDaeGauntletTestController::OnInit()
{
    Super::OnInit();

    // Get tests path.
    const UDaeTestAutomationPluginSettings* TestAutomationPluginSettings =
        GetDefault<UDaeTestAutomationPluginSettings>();

    UE_LOG(LogDaeTest, Display, TEXT("Discovering tests from: %s"),
           *TestAutomationPluginSettings->TestMapPath);

    // Build list of tests (based on FAutomationEditorCommonUtils::CollectTestsByClass).
    FAssetRegistryModule& AssetRegistryModule =
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    TArray<FAssetData> AssetDataArray;

    AssetRegistryModule.Get().SearchAllAssets(true);
    AssetRegistryModule.Get().GetAssetsByClass(UWorld::StaticClass()->GetFName(), AssetDataArray);

    const FString PatternToCheck =
        FString::Printf(TEXT("/%s/"), *TestAutomationPluginSettings->TestMapPath);

    for (auto ObjIter = AssetDataArray.CreateConstIterator(); ObjIter; ++ObjIter)
    {
        const FAssetData& AssetData = *ObjIter;

        FString Filename = FPackageName::LongPackageNameToFilename(AssetData.ObjectPath.ToString());

        if (Filename.Contains(*PatternToCheck))
        {
            FName MapName = AssetData.AssetName;
            MapNames.Add(MapName);

            UE_LOG(LogDaeTest, Display, TEXT("Discovered test: %s"), *MapName.ToString());
        }
    }

    GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::Initialized);
}

void UDaeGauntletTestController::OnPostMapChange(UWorld* World)
{
    Super::OnPostMapChange(World);

    UE_LOG(LogDaeTest, Log, TEXT("UDaeGauntletTestController::OnPostMapChange - World: %s"),
           *World->GetName());

    if (GetCurrentState() != FDaeGauntletStates::LoadingNextMap)
    {
        return;
    }

    GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::DiscoveringTests);
}

void UDaeGauntletTestController::OnTick(float TimeDelta)
{
    if (GetCurrentState() == FDaeGauntletStates::Initialized)
    {
        // If this isn't a test map (e.g. immediately after startup), load first test map now.
        if (!MapNames.Contains(FName(*GetCurrentMap())))
        {
            UE_LOG(LogDaeTest, Log,
                   TEXT("FDaeGauntletStates::Initialized - World is not a test world, "
                        "loading first test world."));

            MapIndex = -1;
            LoadNextTestMap();
            return;
        }
        else
        {
            GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::DiscoveringTests);
        }
    }
    else if (GetCurrentState() == FDaeGauntletStates::LoadingNextMap)
    {
        UE_LOG(LogDaeTest, Display, TEXT("FDaeGauntletStates::LoadingNextMap - Loading map: %s"),
               *MapNames[MapIndex].ToString());

        UGameplayStatics::OpenLevel(this, MapNames[MapIndex]);
    }
    else if (GetCurrentState() == FDaeGauntletStates::DiscoveringTests)
    {
        // Find test suite.
        ADaeTestSuiteActor* TestSuite = nullptr;

        for (TActorIterator<ADaeTestSuiteActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
        {
            TestSuite = *ActorIt;
        }

        if (!IsValid(TestSuite))
        {
            UE_LOG(LogDaeTest, Error,
                   TEXT("FDaeGauntletStates::DiscoveringTests - No "
                        "DaeGauntletTestSuiteActor "
                        "found."));
            LoadNextTestMap();
            return;
        }

        // Start first test.
        GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::Running);

        TestSuite->OnTestSuiteSuccessful.AddDynamic(
            this, &UDaeGauntletTestController::OnTestSuiteFinished);
        TestSuite->OnTestSuiteFailed.AddDynamic(this,
                                                &UDaeGauntletTestController::OnTestSuiteFinished);

        TestSuite->RunAllTests();
    }
}

void UDaeGauntletTestController::LoadNextTestMap()
{
    ++MapIndex;

    // Check if we just want to run a single test.
    FString SingleTestName = ParseCommandLineOption(TEXT("TestName"));

    if (!SingleTestName.IsEmpty())
    {
        while (MapNames.IsValidIndex(MapIndex) && MapNames[MapIndex].ToString() != SingleTestName)
        {
            ++MapIndex;
        }
    }

    if (MapNames.IsValidIndex(MapIndex))
    {
        // Load next test map in next tick. This is to avoid invocation list changes during OnPostMapChange.
        GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::LoadingNextMap);
    }
    else
    {
        // All tests finished.
        UE_LOG(LogDaeTest, Display,
               TEXT("UDaeGauntletTestController::LoadNextTestMap - All tests finished."));

        // Write final test report.
        FDaeJUnitReportWriter JUnitReportWriter;
        FString TestReport =
            JUnitReportWriter.CreateReport(TEXT("DaeGauntletTestController"), Results);
        UE_LOG(LogDaeTest, Log, TEXT("Test report:\r\n%s"), *TestReport);

        FString JUnitReportPath = ParseCommandLineOption(TEXT("JUnitReportPath"));

        if (!JUnitReportPath.IsEmpty())
        {
            UE_LOG(LogDaeTest, Display, TEXT("Writing test report to: %s"), *JUnitReportPath);
            FFileHelper::SaveStringToFile(TestReport, *JUnitReportPath);
        }

        // Finish Gauntlet.
        GetGauntlet()->BroadcastStateChange(FDaeGauntletStates::Finished);

        for (const FDaeTestSuiteResult& Result : Results)
        {
            if (Result.NumFailedTests() > 0)
            {
                EndTest(1);
                return;
            }
        }

        EndTest(0);
    }
}

void UDaeGauntletTestController::OnTestSuiteFinished(ADaeTestSuiteActor* TestSuite)
{
    Results.Add(TestSuite->GetResult());
    LoadNextTestMap();
}

FString UDaeGauntletTestController::ParseCommandLineOption(const FString& Key)
{
    FString Value;
    FParse::Value(FCommandLine::Get(), *Key, Value);
    return Value.Mid(1);
}
