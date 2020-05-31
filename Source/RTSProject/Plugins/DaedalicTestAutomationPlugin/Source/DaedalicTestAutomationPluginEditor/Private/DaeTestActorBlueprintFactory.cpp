#include "DaeTestActorBlueprintFactory.h"
#include "DaeTestActor.h"
#include "DaeTestActorBlueprint.h"
#include <BlueprintEditorSettings.h>
#include <EdGraphSchema_K2.h>
#include <EdGraph/EdGraph.h>
#include <Engine/Blueprint.h>
#include <Engine/BlueprintGeneratedClass.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>

UDaeTestActorBlueprintFactory::UDaeTestActorBlueprintFactory(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDaeTestActorBlueprint::StaticClass();
}

UObject* UDaeTestActorBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent,
                                                         FName Name, EObjectFlags Flags,
                                                         UObject* Context, FFeedbackContext* Warn)
{
    // Create blueprint asset.
    UBlueprint* Blueprint =
        FKismetEditorUtilities::CreateBlueprint(ADaeTestActor::StaticClass(), InParent, Name,
                                                BPTYPE_Normal,
                                                UDaeTestActorBlueprint::StaticClass(),
                                                UBlueprintGeneratedClass::StaticClass());

    if (!IsValid(Blueprint))
    {
        return nullptr;
    }

    // Create special blueprint graph.
    UEdGraph* EdGraph = FBlueprintEditorUtils::CreateNewGraph(Blueprint,
                                                              TEXT("Test Blueprint Graph"),
                                                              UEdGraph::StaticClass(),
                                                              UEdGraphSchema_K2::StaticClass());
    if (Blueprint->UbergraphPages.Num())
    {
        FBlueprintEditorUtils::RemoveGraphs(Blueprint, Blueprint->UbergraphPages);
    }

    FBlueprintEditorUtils::AddUbergraphPage(Blueprint, EdGraph);
    Blueprint->LastEditedDocuments.Add(EdGraph);
    EdGraph->bAllowDeletion = false;

    UBlueprintEditorSettings* Settings = GetMutableDefault<UBlueprintEditorSettings>();

    if (Settings && Settings->bSpawnDefaultBlueprintNodes)
    {
        int32 NodePositionY = 0;
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph,
                                                    FName(TEXT("ReceiveOnAssume")),
                                                    ADaeTestActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph,
                                                    FName(TEXT("ReceiveOnArrange")),
                                                    ADaeTestActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("ReceiveOnAct")),
                                                    ADaeTestActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph,
                                                    FName(TEXT("ReceiveOnAssert")),
                                                    ADaeTestActor::StaticClass(), NodePositionY);
    }

    return Blueprint;
}
