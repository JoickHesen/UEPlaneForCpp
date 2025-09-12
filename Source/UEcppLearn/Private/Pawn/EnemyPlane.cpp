// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/EnemyPlane.h"
#include "BehaviorTree/EnemyAIController.h"
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
	// PawnSensingComp->SetSensingUpdatesEnabled(true);  // 确保感知更新启用
	// PawnSensingComp->SensingInterval = 5.0f;      

	//创建移动组件
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
	// MovementComp->MaxSpeed = 80.0f;
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
	
	return RandomPoint;
}

void AEnemyPlane::OnSeePawn(APawn* EnemyPawn)
{
	if (!EnemyPawn->IsPlayerControlled()) return;

	TargetPawn = EnemyPawn;

	AEnemyAIController* AICon = Cast<AEnemyAIController>(GetController());
	
	if (AICon)
	{
		UE_LOG(LogTemp, Warning, TEXT("感知到玩家，进入追击状态"));
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
	// CurrentPoint = GetRandomPointInPartolBounds();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyPlane::OnSeePawn);
	}
}

// Called every frame
void AEnemyPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
