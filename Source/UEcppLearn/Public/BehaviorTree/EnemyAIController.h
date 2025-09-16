// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AIControllerBase.h"
#include "Components/BoxComponent.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class UECPPLEARN_API AEnemyAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
	virtual void OnPossess(APawn* InPawn) override;

	// UBlackboardComponent* GetBlackboardComp() const {return BlackboardComponent;}

	//生成巡逻点
	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetRandomPatrolPoint() const;
	
protected:
	// //行为树
	// UPROPERTY(EditAnywhere,Category="AI")
	// UBehaviorTree* BehaviorTree;
	//
	// UPROPERTY(EditAnywhere,Category="AI")
	// UBlackboardComponent* BlackboardComponent;

	//重写状态更新函数
	virtual void UpdatePatrolState(float DeltaTime) override;
	virtual void UpdateChaseState(float DeltaTime) override;
	virtual void UpdateLostTargetState(float DeltaTime) override;
	//重写感知处理
	virtual void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus) override;
	virtual void OnTargetLost(AActor* Actor) override;

	
private:
	//获取敌人Pawn
	class AEnemyPlane* GetEnemyPlane() const;

	//巡逻
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	FVector CurrentPatrolPoint;

	FTimerHandle PatrolTimerHandle;

	//更新巡逻点
	void UpdatePatrolPoint();

};
