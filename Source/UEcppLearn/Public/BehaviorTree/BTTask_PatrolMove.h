// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_MoveToInput.h"
#include "BTTask_PatrolMove.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API UBTTask_PatrolMove : public UBTTask_MoveToInput
{
	GENERATED_BODY()

public:
	UBTTask_PatrolMove();

	virtual FVector GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const override;
	
};
