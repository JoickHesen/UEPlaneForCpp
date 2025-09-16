// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/EnemyPlane.h"

AEnemyAIController::AEnemyAIController()
{
	// BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("UBlackboardComponent"));
	CurrentState = EAIState::Patrol;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//设置巡逻点
	UpdatePatrolPoint();

	if (MyBlackboardComponent)
	{
		MyBlackboardComponent->SetValueAsVector("PatrolLocation",CurrentPatrolPoint);
		MyBlackboardComponent->SetValueAsFloat("DesiredDistance",GetEnemyPlane()?GetEnemyPlane()->DesiredDistance:300.0f);
	}
}

void AEnemyAIController::UpdatePatrolState(float DeltaTime)
{
	Super::UpdatePatrolState(DeltaTime);
	//检查是否更新巡逻点
	if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle,this,&AEnemyAIController::UpdatePatrolPoint,5.0f,false);
	}

	//检查是否到达巡逻点
	// if (BlueprintCreatedComponents && GetPawn())
	// {
	// 	
	// }
}

void AEnemyAIController::UpdateChaseState(float DeltaTime)
{
	Super::UpdateChaseState(DeltaTime);
}

void AEnemyAIController::UpdateLostTargetState(float DeltaTime)
{
	Super::UpdateLostTargetState(DeltaTime);
}

void AEnemyAIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetPerceived(Actor, Stimulus);
}

void AEnemyAIController::OnTargetLost(AActor* Actor)
{
	Super::OnTargetLost(Actor);
}

FVector AEnemyAIController::GetRandomPatrolPoint() const
{
	AEnemyPlane* EnemyPlane = GetEnemyPlane();
	if (!EnemyPlane->PatrolBounds)
		return  EnemyPlane->GetActorLocation();
	FVector Origin = EnemyPlane->PatrolBounds->Bounds.Origin;
	FVector Extent = EnemyPlane->PatrolBounds->Bounds.BoxExtent;

	FVector RandomPoint = FMath::RandPointInBox(FBox(Origin - Extent, Origin + Extent));

	return RandomPoint;
}

void AEnemyAIController::UpdatePatrolPoint()
{
	CurrentPatrolPoint = GetRandomPatrolPoint();

	if (MyBlackboardComponent)
	{
		MyBlackboardComponent->SetValueAsVector("PatrolLocation",CurrentPatrolPoint);
	}

	GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
}

class AEnemyPlane* AEnemyAIController::GetEnemyPlane() const
{
	return Cast<AEnemyPlane>(GetPawn());
}

