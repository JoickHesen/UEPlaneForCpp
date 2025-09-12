// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToInput.generated.h"

/**
 * 自定义移动任务节点，继承自UBTTask_MoveTo
 * 使用AddMovementInput而不是导航系统来移动AI角色到达指定目标
 */
UCLASS()
class UECPPLEARN_API UBTTask_MoveToInput : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_MoveToInput();
	//重写执行任务函数，使用AddMovementInput，而不是NavMesh
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//重写Tick函数，处理每帧的移动和旋转逻辑
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//重写任务中止函数
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//重写任务完成函数
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	//到达目标容差半径
	UPROPERTY(Category = Node, EditAnywhere, meta = (ClampMin="0.0",UIMin = "0.0"))
	float MyAcceptanceRadius;
	
	//旋转速度，用于平滑转向目标
	UPROPERTY(Category = Node, EditAnywhere, meta = (ClampMin="0.1",UIMin="0.1"))
	float RotationSpeed;

	//移动速度控制
	UPROPERTY(Category = Node, EditAnywhere, meta = (ClampMin="0.1",UIMin="0.1"))
	float MovementSpeed;


protected:
	//从黑板键获取目标位置
	virtual  FVector GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const;
	//检查是否到达目标
	bool HasReachedTarget(const FVector& Target,const FVector& CurrentLocation) const;
	//平滑旋转到目标方向
	FRotator SmoothRotateToTarget(const FVector& Target,const FVector& CurrentLocation,
								const FRotator& CurrentRotation,float DeltaTime);

	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual UAITask_MoveTo* PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) override;
private:
	//记录是否在移动
	struct FBTMoveToInputTaskMemory
	{
		uint8 bIsMoving : 1;
	};
};
