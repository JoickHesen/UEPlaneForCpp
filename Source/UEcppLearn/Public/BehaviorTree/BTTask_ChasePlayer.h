// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_MoveToInput.h"
#include "BTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API UBTTask_ChasePlayer : public UBTTask_MoveToInput
{
	GENERATED_BODY()

public:
	UBTTask_ChasePlayer();
protected:
	virtual FVector GetTargetLocation(UBehaviorTreeComponent& OwnerComp) const override;
};
