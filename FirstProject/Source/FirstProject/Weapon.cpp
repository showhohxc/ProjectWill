// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"

AWeapon::AWeapon()
{
	m_SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletaMesh"));
	m_SkeletalMesh->SetupAttachment(GetRootComponent());

	m_CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ComBatCollision"));
	m_CombatCollision->SetupAttachment(GetRootComponent());

	bWeaponParticle = false;

	m_WeaponState = EWeaponState::EMS_PICKUP;

	Damage = 25.0f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	m_CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	m_CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if ((m_WeaponState == EWeaponState::EMS_PICKUP) && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			//Equip(Main);
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			//Equip(Main);
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::Equip(class AMain* Char)
{
	if (Char)
	{
		SetInstigator(Char->GetController());

		m_SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		m_SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		m_SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;

			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);
		}

		if(OnEquipSound)
			UGameplayStatics::PlaySound2D(this, OnEquipSound);

		if (!bWeaponParticle)
		{
			ParticleSystem->Deactivate();
		}
	}
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);

		if (Enemy)
		{
			if (Enemy->HitParticles)
			{
				const USkeletalMeshSocket* WeaponSocket = m_SkeletalMesh->GetSocketByName("WeaponSocket");

				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(m_SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0), false);
				}
			}

			if (Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}

			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

	}
}

void AWeapon::ActivateCollison()
{
	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeActivateCollison()
{
	m_CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}