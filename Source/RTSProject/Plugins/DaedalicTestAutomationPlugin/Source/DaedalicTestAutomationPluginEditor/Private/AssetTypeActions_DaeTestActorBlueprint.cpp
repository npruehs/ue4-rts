#include "AssetTypeActions_DaeTestActorBlueprint.h"
#include "DaeTestActorBlueprint.h"
#include "DaeTestActorBlueprintFactory.h"

#define LOCTEXT_NAMESPACE "DaedalicTestAutomationPlugin"

FAssetTypeActions_DaeTestActorBlueprint::FAssetTypeActions_DaeTestActorBlueprint(
    EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{
}

FText FAssetTypeActions_DaeTestActorBlueprint::GetName() const
{
    return NSLOCTEXT("DaedalicTestAutomationPlugin", "AssetTypeActions_DaeTestActorBlueprint",
                     "Test Actor Blueprint");
}

UClass* FAssetTypeActions_DaeTestActorBlueprint::GetSupportedClass() const
{
    return UDaeTestActorBlueprint::StaticClass();
}

uint32 FAssetTypeActions_DaeTestActorBlueprint::GetCategories()
{
    return AssetTypeCategory;
}

UFactory* FAssetTypeActions_DaeTestActorBlueprint::GetFactoryForBlueprintType(
    UBlueprint* InBlueprint) const
{
    return NewObject<UDaeTestActorBlueprintFactory>();
}

#undef LOCTEXT_NAMESPACE
