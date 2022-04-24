// © Acinex Games 2021

#include "AI/Behavior/Task/RTSFaceTargetTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

URTSFaceTargetTask::URTSFaceTargetTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Face Target";
}

EBTNodeResult::Type URTSFaceTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* Controller = Cast<AAIController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	const AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (!IsValid(Pawn) || !IsValid(Target))
	{
		return EBTNodeResult::Aborted;
	}

	FRotator Rotation = Pawn->GetActorRotation();
	Rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Target->GetActorLocation()).Yaw;

	Pawn->SetActorRotation(Rotation);
	return EBTNodeResult::Succeeded;
}

FString URTSFaceTargetTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Target: ").Append(GetSelectedBlackboardKey().ToString());

	return Description;
}
