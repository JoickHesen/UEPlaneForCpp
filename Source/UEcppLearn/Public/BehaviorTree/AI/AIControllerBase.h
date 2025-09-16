// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIModule/Classes/AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"


/**
 * 负责基本的AI功能，AI控制器，负责AI的感知、状态管理、行为树运行等 ，，AI状态逻辑都在本类中实现
 */

UENUM()
enum class EAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Patrol		UMETA(DisplayName = "Patrol"),
	Chase		UMETA(DisplayName = "Chase"),
	Combat		UMETA(DisplayName = "Combat"),
	LostTarget	UMETA(DisplayName = "LostTarget")
};

UCLASS(Abstract,Blueprintable)
class UECPPLEARN_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIControllerBase();

	virtual void OnPossess(APawn* InPawn) override;

	//获取AI当前状态
	UFUNCTION(BlueprintCallable, Category = "AI")
	EAIState GetCurrentState() const {return  CurrentState;}

	//设置AI状态
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetAIState(EAIState NewState);

	//获取黑板组件
	UBlackboardComponent* GetBlackboardComponent() const {return MyBlackboardComponent;}

	//感知目标
	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	// UFUNCTION()
	// virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void OnTargetPerceived(AActor* Actor,FAIStimulus Stimulus);

	//失去视线目标
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void OnTargetLost(AActor* Actor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//初始化感知组件
	virtual void SetupPerceptionSystem(float SightRadius,float LoseSightRadius,float PeripheralVisionAngleDegrees);

	//状态转化逻辑
	virtual void HandleStateTransition(EAIState NewState);

	//状态更新函数
	virtual void UpdateIdleState(float DeltaTime){}
	virtual void UpdatePatrolState(float DeltaTime){}
	virtual void UpdateChaseState(float DeltaTime){}
	virtual void UpdateCombatState(float DeltaTime){}
	virtual void UpdateLostTargetState(float DeltaTime){}

	//检测是否失去目标视线
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual bool HasLostSightOfTarget() const;

	//开始失去目标计时器
	void StartLoseSightTimer();
	//停止失去目标计时器
	void StopLoseSightTimer();
	//失去目标计时器回调
	UFUNCTION()
	void OnLoseSightTimerExpired();

	// 检查目标可见性
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void CheckTargetVisibility();

	//感知组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* MyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	//行为树
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	//黑板组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* MyBlackboardComponent;

	//当前AI状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIState CurrentState;

	//上一AI状态（主要用于返回）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIState PreviousState;

	//当前感知到的目标
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* PerceivedTarget;
	//失去视线后的等待时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float LoseSightCooldown;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle LoseSightTimerHandle;
	bool bHasTargetInSight;
};
