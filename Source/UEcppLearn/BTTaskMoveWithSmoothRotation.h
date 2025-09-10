// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMoveWithSmoothRotation.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API UBTTaskMoveWithSmoothRotation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskMoveWithSmoothRotation(); 

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bIsMoving;
};