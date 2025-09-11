// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeAIAction.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API UBTTaskNodeAIAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNodeAIAction();

	// UPROPERTY(EditAnywhere, Category=Blackboard)
	// struct FBlackboardKeySelector BlackboardKey;
	
	//执行任务
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	

protected:
	UPROPERTY(EditAnywhere)
	float SearchRadius = 2000.0f;
	
private:
	
};
