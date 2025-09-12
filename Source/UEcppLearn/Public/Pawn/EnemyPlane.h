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

	//失去视线后等待时间
	UPROPERTY(EditAnywhere, Category="AI")
	float LoseSightCooldown = 3.0f;

	//生成巡逻随机点
	UFUNCTION(BlueprintCallable, Category="AI")
	FVector GetRandomPointInPartolBounds();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	// UFUNCTION()
	// void OnLoseSight(APawn* Pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
