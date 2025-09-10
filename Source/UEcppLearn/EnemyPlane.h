// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyPlane.generated.h"

UCLASS()
class UECPPLEARN_API AEnemyPlane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPlane();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* EnemyPlaneMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* EnemyPlaneOutCollision;

	UPROPERTY(VisibleAnywhere, Category="AI")
	UPawnSensingComponent* PawnSensingComp;

	//移动组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	class UFloatingPawnMovement* MovementComp;

	// 玩家目标
	UPROPERTY(VisibleAnywhere)
	APawn* TargetPawn;

	// 保持距离
	UPROPERTY(EditAnywhere, Category="AI")
	float DesiredDistance = 0.0f;

	//范围巡逻盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UBoxComponent* PatrolBounds;

	//当前目标点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	FVector CurrentPoint;

	//是否正在转向
	bool bIsTurning;

	//转向速度
	float RotationSpeed = 5.0f;

	//到达点容差范围
	float AcceptanceRadius = 5.0f;

	//生成巡逻随机点
	UFUNCTION(BlueprintCallable, Category="AI")
	FVector GetRandomPointInPartolBounds();

	//移动到下一个点
	UFUNCTION(BlueprintCallable, Category="AI")
	void MoveToNextPoint();

	//检查是否到达目标点
	UFUNCTION(BlueprintCallable, Category="AI")
	bool HasReachedTarget(const FVector& Target) const;

	//平滑转向目标
	UFUNCTION(BlueprintCallable, Category="AI")
	void SmoothRotateToTarget(float DeltaTime);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
