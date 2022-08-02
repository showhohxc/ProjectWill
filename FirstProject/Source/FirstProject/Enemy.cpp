// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Main.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.0f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	m_CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ComBatCollision"));
	m_CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	Health = 75.0f;
	MaxHealth = 100.0f;
	Damage = 10.0f;

	bAttacking = false;

	fAttackMinTime = 0.5f;
	fAttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovemntStatus::EMS_IDLE;

	fDeathDealy = 3.0f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSpherelapBeginOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	bOverlappingCombatSphere = false;

	m_CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	m_CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* main = Cast<AMain>(OtherActor);

		if (main)
		{
			MoveToTarget(main);
		}
	}
}

void AEnemy::AgroSpherelapBeginOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_IDLE);
				
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				Main->SetCombatTarget(this);
				CombatTarget = Main;
				bOverlappingCombatSphere = true;
				Attack(); // == SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_Attacking);
				//SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_Attacking);
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				if (Main->CombatTarget == this)
				{
					Main->SetCombatTarget(nullptr);
				}
				
				bOverlappingCombatSphere = false;
				// 즉사 쫒아감
				//SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_MoveToTarget);
				//MoveToTarget(Main);

				if (EnemyMovementStatus != EEnemyMovemntStatus::EMS_Attacking)
				{
					MoveToTarget(Main);
					CombatTarget = nullptr;
				}

				GetWorldTimerManager().ClearTimer(AttackTimer);
			}
		}
	}
}

void AEnemy::MoveToTarget(class AMain* Target)
{
	SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_MoveToTarget);

	if (AIController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MoveToTarget()"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		// 오브젝트와의 충돌 사이 자간??
		MoveRequest.SetAcceptanceRadius(25.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
		/* 나중에 사용하자 */
		//auto PathPoints = NavPath->GetPathPoints();

		//for (auto Point : PathPoints)
		//{
		//	//FNavLocation
		//	FVector Location = Point.Location;

		//	UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 15.f, 1.5f);
		//}
	}
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			if (Main->HitParticles)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");

				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0), false);
				}
			}

			if (Main->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Main->HitSound);
			}

			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

	}
}

void AEnemy::ActivateCollison()
{
	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

void AEnemy::DeActivateCollison()
{
	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive())
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_Attacking);
		}

		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;

	if (bOverlappingCombatSphere)
	{
		float fAttackTime = FMath::FRandRange(fAttackMinTime, fAttackMaxTime);

		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, fAttackTime);
		//Attack();
	}
}


float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Health - DamageAmount <= 0.0f)
	{
		Health -= DamageAmount;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}

	SetEnemyMovementStatus(EEnemyMovemntStatus::EMS_Dead);

	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, fDeathDealy);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovemntStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}