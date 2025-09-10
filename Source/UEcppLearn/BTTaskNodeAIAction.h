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
	//执行任务
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 
	

protected:
	UPROPERTY(EditAnywhere)
	float SearchRedius = 2000.0f;
	
private:
	
};
