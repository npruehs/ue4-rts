// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AutomationTestFramework/DaeTestAutomationPluginAutomationTestFrameworkIntegration.h"
#include "AssetTypeActions_DaeTestActorBlueprint.h"
#include "AssetTypeActions_DaeTestParameterProviderActorBlueprint.h"
#include "AssetTypeActions_DaeTestSuiteActorBlueprint.h"
#include "DaeTestAutomationPluginSettings.h"
#include "DaedalicTestAutomationPluginEditorClasses.h"
#include "IDaedalicTestAutomationPluginEditor.h"
#include <AssetToolsModule.h>
#include <CoreMinimal.h>
#include <IAssetTypeActions.h>
#include <ISettingsModule.h>
#include <ISettingsSection.h>
#include <Modules/ModuleManager.h>

#define LOCTEXT_NAMESPACE "DaedalicTestAutomationPlugin"

class FDaedalicTestAutomationPluginEditor : public IDaedalicTestAutomationPluginEditor
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    /** Asset catetory for test automation assets. */
    EAssetTypeCategories::Type DaedalicTestAutomationAssetCategory;

    /** Asset type actions registered by this module. */
    TArray<TSharedPtr<IAssetTypeActions>> AssetTypeActions;

    /** Integration with the Unreal Automation Test Framework. */
    FDaeTestAutomationPluginAutomationTestFrameworkIntegration AutomationTestFrameworkIntegration;

    void RegisterAssetTypeAction(class IAssetTools& AssetTools,
                                 TSharedRef<IAssetTypeActions> Action);

    void OnTestMapPathChanged(const FString& NewTestMapPath);
};

IMPLEMENT_MODULE(FDaedalicTestAutomationPluginEditor, DaedalicTestAutomationPluginEditor)

void FDaedalicTestAutomationPluginEditor::StartupModule()
{
    // Register asset types.
    IAssetTools& AssetTools =
        FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    DaedalicTestAutomationAssetCategory =
        AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DaedalicTestAutomationPlugin")),
                                                 NSLOCTEXT("DaedalicTestAutomationPlugin",
                                                           "DaedalicTestAutomationAssetCategory",
                                                           "Test Automation"));

    TSharedRef<IAssetTypeActions> TestActorBlueprintAction = MakeShareable(
        new FAssetTypeActions_DaeTestActorBlueprint(DaedalicTestAutomationAssetCategory));
    RegisterAssetTypeAction(AssetTools, TestActorBlueprintAction);

    TSharedRef<IAssetTypeActions> TestSuiteActorBlueprintAction = MakeShareable(
        new FAssetTypeActions_DaeTestSuiteActorBlueprint(DaedalicTestAutomationAssetCategory));
    RegisterAssetTypeAction(AssetTools, TestSuiteActorBlueprintAction);

    TSharedRef<IAssetTypeActions> TestParameterProviderActorBlueprintAction =
        MakeShareable(new FAssetTypeActions_DaeTestParameterProviderActorBlueprint(
            DaedalicTestAutomationAssetCategory));
    RegisterAssetTypeAction(AssetTools, TestParameterProviderActorBlueprintAction);

    // Register settings.
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        UDaeTestAutomationPluginSettings* TestAutomationPluginSettings =
            GetMutableDefault<UDaeTestAutomationPluginSettings>();

        ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
            "Project", "Plugins", "DaedalicTestAutomationPlugin",
            NSLOCTEXT("DaedalicTestAutomationPlugin", "DaeTestAutomationPluginSettings.DisplayName",
                      "Daedalic Test Automation Plugin"),
            NSLOCTEXT("DaedalicTestAutomationPlugin", "DaeTestAutomationPluginSettings.Description",
                      "Configure the discovery of automated tests."),
            TestAutomationPluginSettings);

        TestAutomationPluginSettings->OnTestMapPathChanged.AddRaw(
            this, &FDaedalicTestAutomationPluginEditor::OnTestMapPathChanged);

        OnTestMapPathChanged(TestAutomationPluginSettings->TestMapPath);
    }
}

void FDaedalicTestAutomationPluginEditor::ShutdownModule()
{
    // Unregister asset types.
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetToolsModule =
            FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (auto& AssetTypeAction : AssetTypeActions)
        {
            if (AssetTypeAction.IsValid())
            {
                AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
            }
        }
    }

    AssetTypeActions.Empty();

    // Unregister settings.
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Editor", "Plugins", "DaedalicTestAutomationPlugin");
    }
}

void FDaedalicTestAutomationPluginEditor::RegisterAssetTypeAction(
    class IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
    AssetTools.RegisterAssetTypeActions(Action);
    AssetTypeActions.Add(Action);
}

void FDaedalicTestAutomationPluginEditor::OnTestMapPathChanged(const FString& NewTestMapPath)
{
    // Discover tests.
    AutomationTestFrameworkIntegration.SetTestMapPath(NewTestMapPath);
}

#undef LOCTEXT_NAMESPACE
