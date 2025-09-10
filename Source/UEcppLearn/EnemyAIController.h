// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;

	UBlackboardComponent* GetBlackboardComp() const {return BlackboardComponent;}
	
protected:
	//行为树
	UPROPERTY(EditAnywhere,Category="AI")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere,Category="AI")
	UBlackboardComponent* BlackboardComponent;
private:

};
