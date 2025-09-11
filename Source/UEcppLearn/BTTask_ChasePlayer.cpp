// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("ChasePlayer");

	MyAcceptanceRadius = 300.0f;
	RotationSpeed = 8.0f;
	MovementSpeed = 10.0f;
}

FVector UBTTask_ChasePlayer::GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return FVector::ZeroVector;
	}

	UObject* KeyValue = BlackboardComponent->GetValue<UBlackboardKeyType_Object>(
		BlackboardComponent->GetKeyID("TargetActor"));
	AActor* TargetActor = Cast<AActor>(KeyValue);

	if (TargetActor)
	{
		return TargetActor->GetActorLocation();
	}
	
	return FVector::ZeroVector;
}
