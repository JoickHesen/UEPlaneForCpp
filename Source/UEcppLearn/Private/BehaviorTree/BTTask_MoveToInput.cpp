// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_MoveToInput.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_MoveToInput::UBTTask_MoveToInput()
{
	NodeName = TEXT("MoveToInput");

	bNotifyTick = true;

	RotationSpeed = 5.0f;
	MovementSpeed = 5.0f;
	MyAcceptanceRadius = 50.0f;

	//禁用导航相关功能
	bUsePathfinding = false;
	bProjectGoalLocation = false;
}

//初始化移动状态
EBTNodeResult::Type UBTTask_MoveToInput::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//调用父类初始化黑板基础设施
	EBTNodeResult::Type ParentResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (ParentResult != EBTNodeResult::InProgress)
	{
		return ParentResult;
	}

	FBTMoveToInputTaskMemory* MyMemory = reinterpret_cast<FBTMoveToInputTaskMemory*>(NodeMemory);
	MyMemory->bIsMoving = true;

	return EBTNodeResult::InProgress;
}


//处理移动和旋转逻辑
void UBTTask_MoveToInput::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//获取AIController和Pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}

	//获取目标位置
	FVector TargetLocation = GetTargetLocation(OwnerComp);
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	FRotator CurrentRotation = ControlledPawn->GetActorRotation();

	//检查是否到达目标
	if (HasReachedTarget(TargetLocation, CurrentLocation))
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return;
	}

	//计算目标方向
	FVector TargetDirection = (TargetLocation - CurrentLocation).GetSafeNormal();

	if (!TargetDirection.IsNearlyZero() && !CurrentRotation.Vector().Equals(TargetDirection,0.1f))
	{
		//平滑旋转到目标方向
		FRotator NewRotation = SmoothRotateToTarget(
			TargetLocation,
			CurrentLocation,
			CurrentRotation,
			DeltaSeconds
		);

		//应用旋转
		ControlledPawn->SetActorRotation(NewRotation);
	}

	//使用AddMovementInput向前移动
	FVector MovementDirection = ControlledPawn->GetActorForwardVector();
	float Speed = MovementSpeed * 100.0f;	
	ControlledPawn->AddMovementInput(MovementDirection,10.0f);
}

EBTNodeResult::Type UBTTask_MoveToInput::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveToInputTaskMemory* MyMemory = reinterpret_cast<FBTMoveToInputTaskMemory*>(NodeMemory);
	MyMemory->bIsMoving = false;
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_MoveToInput::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	FBTMoveToInputTaskMemory* MyMemory = reinterpret_cast<FBTMoveToInputTaskMemory*>(NodeMemory);
	MyMemory->bIsMoving = false;
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

FVector UBTTask_MoveToInput::GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return FVector::ZeroVector;
	}

	//根据黑板键类型获取目标位置
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(KeyValue);
		if (TargetActor)
		{
			return TargetActor->GetActorLocation();
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		return BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	}
	return FVector::ZeroVector;
}

bool UBTTask_MoveToInput::HasReachedTarget(const FVector& Target, const FVector& CurrentLocation) const
{
	float Distance = FVector::Dist(Target, CurrentLocation);
	return Distance < MyAcceptanceRadius;
}

FRotator UBTTask_MoveToInput::SmoothRotateToTarget(const FVector& Target, const FVector& CurrentLocation,
	const FRotator& CurrentRotation, float DeltaTime)
{
	// return FRotator::ZeroRotator;
	//计算目标方向
	FVector Direction = (Target - CurrentLocation).GetSafeNormal();
	if (CurrentRotation.Vector().Equals(Direction, 0.1f))
	{
		return CurrentRotation;
	}

	FRotator TargetRotation = Direction.Rotation();

	//使用插值平滑旋转
	FRotator NewRotator =  FMath::RInterpTo(
		CurrentRotation,
		TargetRotation,
		DeltaTime,
		RotationSpeed
		);
	return NewRotator;
}

EBTNodeResult::Type UBTTask_MoveToInput::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type::InProgress;
}

UAITask_MoveTo* UBTTask_MoveToInput::PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask,
	FAIMoveRequest& MoveRequest)
{
	return nullptr;
}
