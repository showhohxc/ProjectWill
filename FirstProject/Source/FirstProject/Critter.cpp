// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponenet")); // Root 컴포넌트 지정		// CreateDefaultSubobject == New
	m_MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponenet"));	// Create 안하면 에러생겨서 팅김 
	m_MeshComponent2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponenet2"));	// Create 안하면 에러생겨서 팅김 
	//SetRootComponent(m_MeshComponent);
	m_MeshComponent->SetupAttachment(GetRootComponent());	// m_MeshComponent를 RootComponent의 자식 컴포컴포넌트로 지정

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(GetRootComponent());
	m_Camera->SetRelativeLocation(FVector(-300, 0, 300));
	m_Camera->SetRelativeRotation(FRotator(-45, 0, 0));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	CurrentVelocity = FVector(0);
	fMaxSpeed = 100.0f;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocal = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocal);
}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACritter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight);

}

void ACritter::MoveForward(float value)
{
	CurrentVelocity.Y = FMath::Clamp(value, -1.f, 1.f) * fMaxSpeed;
}

void ACritter::MoveRight(float value)
{
	CurrentVelocity.X = FMath::Clamp(value, -1.f, 1.f) * fMaxSpeed;
}

void ACritter::MousePitch(float AxValue)
{
	
}

void ACritter::MouseYaw(float AxValue)
{

}

