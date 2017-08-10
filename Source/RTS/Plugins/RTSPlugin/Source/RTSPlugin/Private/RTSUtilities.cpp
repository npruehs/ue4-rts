#include "RTSPluginPrivatePCH.h"
#include "RTSUtilities.h"


URTSUtilities::URTSUtilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


template<class T>
T* URTSUtilities::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass)
{
	return (T*)FindDefaultComponentByClass(InActorClass, T::StaticClass());
}

UActorComponent* URTSUtilities::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass)
{
	UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);

	if (!ActorBlueprintGeneratedClass)
	{
		return nullptr;
	}

	const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

	for (USCS_Node* Node : ActorBlueprintNodes)
	{
		if (Node->ComponentClass == InComponentClass)
		{
			return Node->ComponentTemplate;
		}
	}

	return nullptr;
}
