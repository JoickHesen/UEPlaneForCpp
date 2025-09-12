// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/AI/AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Pawn.h"

// Sets default values
AAIControllerBase::AAIControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建感知组件
	MyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("MyPerceptionComponent"));
	//创建黑板组件
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	CurrentState = EAIState::Patrol;
	PreviousState = EAIState::Patrol;
	PerceivedTarget = nullptr;
	LoseSightCooldown = 3.0f;
	bHasTargetInSight = false;
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetupPerceptionSystem(2000.f,2500.f,60.f);

	if (BehaviorTree && BlackboardComponent)
	{
		UseBlackboard(BehaviorTree->BlackboardAsset,BlackboardComponent);
		RunBehaviorTree(BehaviorTree);
	}
}

void AAIControllerBase::SetAIState(EAIState NewState)
{
	if (CurrentState != NewState)
	{
		PreviousState = CurrentState;
		CurrentState = NewState;
		HandleStateTransition(NewState);

		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsEnum("AIState",(uint8)NewState);
		}
	}
}

void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		MyPerceptionComponent->GetActorsPerception(Actor,PerceptionInfo);

		for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
			{
				OnTargetPerceived(Actor,Stimulus);
			}
		}
	}
}

// void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
// {
// 	if (Stimulus.WasSuccessfullySensed())
// 	{
// 		OnTargetPerceived(Actor,Stimulus);
// 	}
// 	else
// 	{
// 		OnTargetLost(Actor);
// 	}
// }

void AAIControllerBase::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		PerceivedTarget = Actor;
		bHasTargetInSight = true;

		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsObject("TargetActor",Actor);
			BlackboardComponent->SetValueAsBool("bHasLineOfSight",true);
		}

		SetAIState(EAIState::Chase);
	}
	else
	{
		OnTargetLost(Actor);
	}
}

void AAIControllerBase::OnTargetLost(AActor* Actor)
{
	if (Actor == PerceivedTarget && CurrentState == EAIState::Chase)
	{
		bHasTargetInSight = false;
		StartLoseSightTimer();
		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsBool("bHasLineOfSight",false);
		}
	}
}

// Called when the game starts or when spawned
void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIControllerBase::SetupPerceptionSystem(float SightRadius,
											float LoseSightRadius,
											float PeripheralVisionAngleDegrees)
{
	if (!MyPerceptionComponent) return;

	//感知组件配置
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		MyPerceptionComponent->ConfigureSense(*SightConfig);
		MyPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	//感知回调
	MyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this,&AAIControllerBase::OnPerceptionUpdated);
	// PerceptionComponent->OnPerceptionUpdated.AddDynamic(this,&AAIControllerBase::OnTargetPerceptionUpdated);
}

void AAIControllerBase::HandleStateTransition(EAIState NewState)
{
	switch (NewState)
	{
	case EAIState::Chase:
		//开始追逐时停止视线计时器
		StopLoseSightTimer();
		break;
	case EAIState::Patrol:
		if (BlackboardComponent)
		{
			BlackboardComponent->ClearValue("TargetActor");
			BlackboardComponent->ClearValue("bHasLineOfSight");
		}
		PerceivedTarget = nullptr;
		bHasTargetInSight = false;
		StartLoseSightTimer();
		break;
	case EAIState::LostTarget:
		StartLoseSightTimer();
		break;;
	}
}

bool AAIControllerBase::HasLostSightOfTarget() const
{
	return !bHasTargetInSight;
}

void AAIControllerBase::StartLoseSightTimer()
{
	if (CurrentState == EAIState::Chase && !GetWorld()->GetTimerManager().IsTimerActive(LoseSightTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(LoseSightTimerHandle,this,
			&AAIControllerBase::OnLoseSightTimerExpired,LoseSightCooldown,false);
	}
}

void AAIControllerBase::StopLoseSightTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(LoseSightTimerHandle);
}

void AAIControllerBase::OnLoseSightTimerExpired()
{
	if (CurrentState == EAIState::Chase && HasLostSightOfTarget())
	{
		//切换到丢失目标状态
		SetAIState(EAIState::LostTarget);

		//短暂等待后返回巡逻
		FTimerHandle ReturnToPatrolTimer;
		GetWorld()->GetTimerManager().SetTimer(ReturnToPatrolTimer,[this]()
		{
			SetAIState(EAIState::Patrol);
		},1.0f,false);
	}
}

// Called every frame
void AAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//根据当前状态更新行为
	switch (CurrentState)
	{
	case EAIState::Idle:
		UpdateIdleState(DeltaTime);
		break;
	case EAIState::Patrol:
		UpdatePatrolState(DeltaTime);
		break;
	case EAIState::Chase:
		UpdateChaseState(DeltaTime);
		CheckTargetVisibility();
		break;
	case EAIState::Combat:
		UpdateCombatState(DeltaTime);
		break;
	case EAIState::LostTarget:
		UpdateLostTargetState(DeltaTime);
		break;
	}
}

void AAIControllerBase::CheckTargetVisibility()
{
	if (CurrentState == EAIState::Chase && PerceivedTarget)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			//简单距离检查
			float Distance = FVector::Distance(ControlledPawn->GetActorLocation(),PerceivedTarget->GetActorLocation());
			if (Distance > SightConfig->SightRadius && bHasTargetInSight)
			{
				//超出视野范围
				OnTargetLost(PerceivedTarget);
			}
			else if (Distance <= SightConfig->SightRadius && !bHasTargetInSight)
			{
				//目标重新进入视野
				FAIStimulus Stimulus;
				// Stimulus.WasSuccessfullySensed = true;
				OnTargetPerceived(PerceivedTarget,Stimulus);
			}
		}
		StartLoseSightTimer();
	}
}

