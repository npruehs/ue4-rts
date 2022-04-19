#include "Libraries/RTSConstructionLibrary.h"

#include "Construction/RTSBuilderComponent.h"


int32 URTSConstructionLibrary::GetConstructableBuildingIndex(AActor* Builder, TSubclassOf<AActor> BuildingClass)
{
	if (!IsValid(Builder))
	{
		return INDEX_NONE;
	}

	const URTSBuilderComponent* BuilderComponent = Builder->FindComponentByClass<URTSBuilderComponent>();

	if (!IsValid(BuilderComponent))
	{
		return INDEX_NONE;
	}

	return BuilderComponent->GetConstructibleBuildingClasses().IndexOfByKey(BuildingClass);
}

TSubclassOf<AActor> URTSConstructionLibrary::GetConstructableBuildingClass(AActor* Builder, int32 BuildingIndex)
{
	if (!IsValid(Builder))
	{
		return nullptr;
	}

	const URTSBuilderComponent* BuilderComponent = Builder->FindComponentByClass<URTSBuilderComponent>();

	if (!IsValid(BuilderComponent))
	{
		return nullptr;
	}

	TArray<TSubclassOf<AActor>> ConstructableBuildings = BuilderComponent->GetConstructibleBuildingClasses();
	return ConstructableBuildings.IsValidIndex(BuildingIndex) ? ConstructableBuildings[BuildingIndex] : nullptr;
}
