// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskMoveWithSmoothRotation.h"

#include "AIController.h"
#include "EnemyPlane.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskMoveWithSmoothRotation::UBTTaskMoveWithSmoothRotation()
{
	NodeName = TEXT("MoveWithSmoothRotation");
	bTickIntervals = false;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTaskMoveWithSmoothRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsMoving = true;
	return EBTNodeResult::InProgress;
}

// void UBTTaskMoveWithSmoothRotation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
// {
// 	AAIController* AIController = OwnerComp.GetAIOwner();
// 	if (!AIController)
// 	{
// 		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
// 		return;
// 	}
//
// 	AEnemyPlane* EnemyPlane = Cast<AEnemyPlane>(AIController->GetPawn());
// 	if (!EnemyPlane)
// 	{
// 		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
// 		return;
// 	}
//
// 	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
// 	FVector TargetLocation = Blackboard->GetValueAsVector(TEXT("PatrolLocation"));
//
// 	//检查是否到达目标
// 	if (EnemyPlane->HasReachedTarget(TargetLocation))
// 	{
// 		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
// 		return;
// 	}
//
// 	//平滑处理
// 	EnemyPlane->SmoothRotateToTarget(DeltaSeconds);
//
// 	//只有当前向目标时才向前移动
// 	if (!EnemyPlane->bIsTurning)
// 	{
// 		FVector Direction = EnemyPlane->GetActorForwardVector();
// 		EnemyPlane->AddMovementInput(Direction,1.0f);
// 	}
// }
