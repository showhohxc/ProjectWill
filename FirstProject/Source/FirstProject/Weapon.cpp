// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
	m_SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletaMesh"));
	m_SkeletalMesh->SetupAttachment(GetRootComponent());
	bWeaponParticle = false;

	m_WeaponState = EWeaponState::EMS_PICKUP;
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