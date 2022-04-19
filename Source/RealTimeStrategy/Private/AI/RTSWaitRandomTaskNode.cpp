#include "TimerManager.h"
#include "AI/RTSWaitRandomTask.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


EBTNodeResult::Type URTSWaitRandomTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FTimerHandle Handle;

	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=, &OwnerComp]
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});

	const float Delay = UKismetMathLibrary::RandomFloatInRange(.2F, 3);

	OwnerComp.GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, Delay, false);

	return EBTNodeResult::InProgress;
}