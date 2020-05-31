#include "AssetTypeActions_DaeTestSuiteActorBlueprint.h"
#include "DaeTestSuiteActorBlueprint.h"
#include "DaeTestSuiteActorBlueprintFactory.h"

#define LOCTEXT_NAMESPACE "DaedalicTestAutomationPlugin"

FAssetTypeActions_DaeTestSuiteActorBlueprint::FAssetTypeActions_DaeTestSuiteActorBlueprint(
    EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{
}

FText FAssetTypeActions_DaeTestSuiteActorBlueprint::GetName() const
{
    return NSLOCTEXT("DaedalicTestAutomationPlugin", "AssetTypeActions_DaeTestSuiteActorBlueprint",
                     "Test Suite Actor Blueprint");
}

UClass* FAssetTypeActions_DaeTestSuiteActorBlueprint::GetSupportedClass() const
{
    return UDaeTestSuiteActorBlueprint::StaticClass();
}

uint32 FAssetTypeActions_DaeTestSuiteActorBlueprint::GetCategories()
{
    return AssetTypeCategory;
}

UFactory* FAssetTypeActions_DaeTestSuiteActorBlueprint::GetFactoryForBlueprintType(
    UBlueprint* InBlueprint) const
{
    return NewObject<UDaeTestSuiteActorBlueprintFactory>();
}

#undef LOCTEXT_NAMESPACE
