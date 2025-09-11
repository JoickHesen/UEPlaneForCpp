// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolMove.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PatrolMove::UBTTask_PatrolMove()
{
	NodeName = TEXT("PatrolMove");

	//巡逻参数
	MyAcceptanceRadius = 5000.0f;
	RotationSpeed = 5.0f;
	MovementSpeed = 5.0f;
}

FVector UBTTask_PatrolMove::GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return FVector::ZeroVector;
	}
	
	return BlackboardComponent->GetValueAsVector("PatrolLocation");
}
