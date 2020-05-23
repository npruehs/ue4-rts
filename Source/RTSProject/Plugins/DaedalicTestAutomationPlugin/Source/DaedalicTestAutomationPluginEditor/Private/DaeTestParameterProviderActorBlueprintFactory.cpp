#include "DaeTestParameterProviderActorBlueprintFactory.h"
#include "DaeTestParameterProviderActor.h"
#include "DaeTestParameterProviderActorBlueprint.h"
#include <BlueprintEditorSettings.h>
#include <EdGraphSchema_K2.h>
#include <EdGraph/EdGraph.h>
#include <Engine/Blueprint.h>
#include <Engine/BlueprintGeneratedClass.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>

UDaeTestParameterProviderActorBlueprintFactory::UDaeTestParameterProviderActorBlueprintFactory(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDaeTestParameterProviderActorBlueprint::StaticClass();
}

UObject* UDaeTestParameterProviderActorBlueprintFactory::FactoryCreateNew(
    UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context,
    FFeedbackContext* Warn)
{
    // Create blueprint asset.
    return FKismetEditorUtilities::CreateBlueprint(
        ADaeTestParameterProviderActor::StaticClass(), InParent, Name, BPTYPE_Normal,
        UDaeTestParameterProviderActorBlueprint::StaticClass(),
        UBlueprintGeneratedClass::StaticClass());
}
