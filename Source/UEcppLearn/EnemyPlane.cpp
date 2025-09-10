// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlane.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AEnemyPlane::AEnemyPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyPlaneOutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));

	RootComponent = EnemyPlaneOutCollision;
	EnemyPlaneOutCollision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	// EnemyPlaneOutCollision->SetSimulatePhysics(true);
	// EnemyPlaneOutCollision->BodyInstance.bLockXRotation = true;
	// EnemyPlaneOutCollision->BodyInstance.bLockYRotation = true;

	EnemyPlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	EnemyPlaneMesh->SetupAttachment(RootComponent);
	EnemyPlaneMesh->SetRelativeRotation(FRotator(0.0f,-90.0f,0.0f));
	// EnemyPlaneMesh->SetupAttachment(OutCollision);
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SightRadius = 1500.f;          // 感知范围
	PawnSensingComp->SetPeripheralVisionAngle(45.f);

	//创建移动组件
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
	MovementComp->MaxSpeed = 80.0f;
	MovementComp->Acceleration = 1000.0f;
	MovementComp->Deceleration = 1000.0f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyAIController::StaticClass();

	//创建巡逻范围盒子
	PatrolBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolBounds"));
	// PatrolBounds->SetupAttachment(RootComponent);
	PatrolBounds->SetBoxExtent(FVector(1000.f, 1000.f, 1000.f));
	PatrolBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);//不需要碰撞
}

FVector AEnemyPlane::GetRandomPointInPartolBounds()
{
	if (!PatrolBounds)
		return GetActorLocation();

	FVector Origin = PatrolBounds->Bounds.Origin;
	FVector Extent = PatrolBounds->Bounds.BoxExtent;

	//生成随机点
	FVector RandomPoint = FMath::RandPointInBox(FBox(Origin - Extent, Origin + Extent));
	// UE_LOG(LogTemp, Warning, TEXT("RandomPoint: %f"), RandomPoint);

	//确保网格在导航网格上
	// UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	// if (NavSys)
	// {
	// 	FNavLocation NavLocation;
	// 	if (NavSys->ProjectPointToNavigation(RandomPoint, NavLocation))
	// 	{
	// 		RandomPoint = NavLocation.Location;
	// 	}
	// }
	
	return RandomPoint;
}

void AEnemyPlane::MoveToNextPoint()
{
	CurrentPoint = GetRandomPointInPartolBounds();
}

bool AEnemyPlane::HasReachedTarget(const FVector& Target) const
{
	return FVector::Dist(GetActorLocation(),Target)< AcceptanceRadius;
}

void AEnemyPlane::SmoothRotateToTarget(float DeltaTime)
{
	//计算目标方向
	FVector TargetDirection = (CurrentPoint - GetActorLocation()).GetSafeNormal();
	// FVector TargetDirection = CurrentPoint - GetActorLocation();
	
	//如果已经面向目标，不需要旋转
	if (GetActorForwardVector().Equals(TargetDirection, 0.1f))
	{
		bIsTurning = true;
		return;
	}

	bIsTurning = true;

	//计算目标旋转
	FRotator TargetRotation = TargetDirection.Rotation();

	//平滑插值当前旋转到目标旋转
	FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(),
		TargetRotation,
		DeltaTime,
		RotationSpeed
		);
	//应用旋转
	// NewRotation.Pitch = 0;
	// NewRotation.Yaw = 0;
	SetActorRotation(NewRotation);
}

void AEnemyPlane::OnSeePawn(APawn* EnemyPawn)
{
	if (!EnemyPawn->IsPlayerControlled()) return;

	TargetPawn = EnemyPawn;

	AEnemyAIController* AICon = Cast<AEnemyAIController>(GetController());

	// OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), TargetPawn);
	// AICon->GetBlackboardComponent()->SetValueAsObject("TargetActor", EnemyPawn);

	// if (AICon && AICon->GetBlackboardComponent())
	// {
	// 	UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();
	//
	// 	if (BlackboardComp->GetKeyID(TEXT("TargetActor"))!=FBlackboard::InvalidKey)
	// 	{
	// 		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), TargetPawn);
	// 		BlackboardComp->SetValueAsFloat("DesiredDistance", DesiredDistance);
	// 		UE_LOG(LogTemp, Warning, TEXT("Successfully set TargetActor in blackboard"));
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("TargetActor key does not exist in blackboard!"));
	// 	}
	// }
	
	if (AICon)
	{
		// 更新黑板，通知行为树进入追击状态
		AICon->GetBlackboardComponent()->SetValueAsObject("TargetActor", EnemyPawn);
		AICon->GetBlackboardComponent()->SetValueAsFloat("DesiredDistance", DesiredDistance);
	}
}

// Called when the game starts or when spawned
void AEnemyPlane::BeginPlay()
{
	Super::BeginPlay();

	//设置初始目标点
	CurrentPoint = GetRandomPointInPartolBounds();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyPlane::OnSeePawn);
	}
}

// Called every frame
void AEnemyPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//如果在追击玩家，不执行巡逻
	if (TargetPawn)
	{
		return;
	}

	//检查是否到达目标点
	if (HasReachedTarget(CurrentPoint))
	{
		MoveToNextPoint();
	}
	else
	{
		//平滑转向目标
		SmoothRotateToTarget(DeltaTime);
		//向前移动
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, 1.0f);
	}
}

// Called to bind functionality to input
void AEnemyPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
