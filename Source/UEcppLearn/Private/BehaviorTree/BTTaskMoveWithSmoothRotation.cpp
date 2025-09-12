// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTaskMoveWithSmoothRotation.h"

#include "AIController.h"
#include "Pawn/EnemyPlane.h"
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
