#include "AutomationTest.h"

#include "Editor.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"
#include "Tests/AutomationEditorCommon.h"

#include "RTSPawnAIController.h"

#include "RTSAutomationCommon.h"


DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FRTSIssueMoveOrderCommand, FName, ActorTag, FVector, TargetLocation);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRTSIssueMoveOrderTest, "RealTimeStrategy.Orders.IssueMoveOrder", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)


bool FRTSIssueMoveOrderCommand::Update()
{
    AActor* Actor = FRTSAutomationCommon::GetActorWithTag(ActorTag);
    APawn* Pawn = Cast<APawn>(Actor);

    if (!IsValid(Pawn))
    {
        return false;
    }

    ARTSPawnAIController* PawnAIController = Cast<ARTSPawnAIController>(Pawn->Controller);

    if (!IsValid(PawnAIController))
    {
        return false;
    }

    PawnAIController->IssueMoveOrder(TargetLocation);

    return true;
}

bool FRTSIssueMoveOrderTest::RunTest(const FString& Parameters)
{
    // ARRANGE.
    FRTSAutomationCommon::LatentLoadMapAndStartPIE(TEXT("/RealTimeStrategyTests/Maps/TEST-RTSIssueMoveOrder"));

    // ACT.
    const FName ActorTag = FName(TEXT("TestPawn"));
    const FVector TargetLocation = FVector(1000.0f, 0.0f, 0.0f);

    ADD_LATENT_AUTOMATION_COMMAND(FRTSIssueMoveOrderCommand(ActorTag, TargetLocation));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

    // ASSERT.
    ADD_LATENT_AUTOMATION_COMMAND(FRTSTestActorLocationXYCommand(ActorTag, TargetLocation));
    ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());

    return true;
}
