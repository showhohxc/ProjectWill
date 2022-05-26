// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_Mesh;

	m_vStartPoint = FVector(0);
	m_vEndPoint = FVector(0);

	m_bInterPing = false;

	m_fInterSpeed = 4.0f;
	m_fInterTime = 1.0f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	m_vStartPoint = GetActorLocation();
	m_vEndPoint += m_vStartPoint;

	m_bInterPing = false;

	GetWorldTimerManager().SetTimer(m_InterpTimer, this, &AFloatingPlatform::ToggleInterPing, m_fInterTime);

	m_fDistance = (m_vEndPoint - m_vStartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bInterPing)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, m_vEndPoint, DeltaTime, m_fInterSpeed);
		SetActorLocation(Interp);

		float fDisanceTraveld = (GetActorLocation() - m_vStartPoint).Size();

		if (m_fDistance - fDisanceTraveld <= 0.1f)
		{
			ToggleInterPing();
			GetWorldTimerManager().SetTimer(m_InterpTimer, this, &AFloatingPlatform::ToggleInterPing, m_fInterTime);
			SwapVectors(m_vStartPoint, m_vEndPoint);
		}
	}
}

void AFloatingPlatform::ToggleInterPing()
{
	m_bInterPing = !m_bInterPing;
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}

