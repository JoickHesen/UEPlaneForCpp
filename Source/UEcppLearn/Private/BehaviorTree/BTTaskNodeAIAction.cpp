// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTaskNodeAIAction.h"

#include "AIController.h"
#include "Pawn/EnemyPlane.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNodeAIAction::UBTTaskNodeAIAction()
{
	NodeName = TEXT("Find Patrol Location");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTaskNodeAIAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AEnemyPlane* EnemyPlane = Cast<AEnemyPlane>(AICon->GetPawn());
	if (!EnemyPlane)
		return  EBTNodeResult::Failed;

	//使用飞机自身的巡逻点生产随机点
	FVector RandomPoint = EnemyPlane->GetRandomPointInPartolBounds();

	//写入黑板
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), RandomPoint);

	return EBTNodeResult::Succeeded;
}
