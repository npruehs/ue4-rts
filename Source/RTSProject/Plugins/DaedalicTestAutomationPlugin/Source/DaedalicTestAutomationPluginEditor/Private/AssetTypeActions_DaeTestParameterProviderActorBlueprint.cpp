#include "AssetTypeActions_DaeTestParameterProviderActorBlueprint.h"
#include "DaeTestParameterProviderActorBlueprint.h"
#include "DaeTestParameterProviderActorBlueprintFactory.h"

#define LOCTEXT_NAMESPACE "DaedalicTestAutomationPlugin"

FAssetTypeActions_DaeTestParameterProviderActorBlueprint::
    FAssetTypeActions_DaeTestParameterProviderActorBlueprint(
        EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{
}

FText FAssetTypeActions_DaeTestParameterProviderActorBlueprint::GetName() const
{
    return NSLOCTEXT("DaedalicTestAutomationPlugin",
                     "AssetTypeActions_DaeTestParameterProviderActorBlueprint",
                     "Test Parameter Provider Actor Blueprint");
}

UClass* FAssetTypeActions_DaeTestParameterProviderActorBlueprint::GetSupportedClass() const
{
    return UDaeTestParameterProviderActorBlueprint::StaticClass();
}

uint32 FAssetTypeActions_DaeTestParameterProviderActorBlueprint::GetCategories()
{
    return AssetTypeCategory;
}

UFactory* FAssetTypeActions_DaeTestParameterProviderActorBlueprint::GetFactoryForBlueprintType(
    UBlueprint* InBlueprint) const
{
    return NewObject<UDaeTestParameterProviderActorBlueprintFactory>();
}

#undef LOCTEXT_NAMESPACE
