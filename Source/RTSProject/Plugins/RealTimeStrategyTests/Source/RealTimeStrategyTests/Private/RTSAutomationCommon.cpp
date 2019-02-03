#include "RTSAutomationCommon.h"

#include "Editor.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"
#include "Tests/AutomationEditorCommon.h"


AActor* FRTSAutomationCommon::GetActorWithTag(const FName& ActorTag)
{
    UWorld* World = GEditor->PlayWorld;

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithTag(World, ActorTag, Actors);

    for (AActor* Actor : Actors)
    {
        return Actor;
    }

    return nullptr;
}

void FRTSAutomationCommon::LatentLoadMapAndStartPIE(const FString& MapName)
{
    ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(MapName));
    ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
}

bool FRTSTestActorLocationXYCommand::Update()
{
    AActor* Actor = FRTSAutomationCommon::GetActorWithTag(ActorTag);
    
    FAutomationTestFramework::Get().GetCurrentTest()->TestEqual(TEXT("ActorLocation.X"), Actor->GetActorLocation().X, ExpectedLocation.X);
    FAutomationTestFramework::Get().GetCurrentTest()->TestEqual(TEXT("ActorLocation.Y"), Actor->GetActorLocation().Y, ExpectedLocation.Y);

    return true;
}
