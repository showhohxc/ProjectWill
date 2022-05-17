// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponenet"));
	SetRootComponent(m_MeshComponent);
	//m_MeshComponent->SetupAttachment(GetRootComponent());

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SetRootComponent(m_SphereComponent);
	m_SphereComponent->SetupAttachment(GetRootComponent());

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(GetRootComponent());
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	
	if (ParticleAsset.Succeeded())
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_SpringArm->SetupAttachment(GetRootComponent());
	m_SpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
	m_SpringArm->TargetArmLength = 400.f;
	m_SpringArm->bEnableCameraLag = true;
	m_SpringArm->CameraLagSpeed = 3.f;

	m_SphereComponent->InitSphereRadius(40.f);
	m_SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_Camera->SetupAttachment(m_SpringArm, USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;		// OurMovementComponent는 루트컴포넌트를 제어한다. 우리의 움직임 구성요소가 움직임을 적용해야할때 적용

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	
	if (MeshComponentAsset.Succeeded())
	{
		m_MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		m_MeshComponent->SetRelativeLocation(FVector(0, 0, -40.f));
		m_MeshComponent->SetWorldScale3D(FVector(0.8, 0.8, 0.8));
	}

	vec_CameraInput = FVector2D(0.f, 0.f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot_NewRotation = GetActorRotation();
	rot_NewRotation.Yaw += vec_CameraInput.X;
	//rot_NewRotation.Pitch += vec_CameraInput.Y;
	SetActorRotation(rot_NewRotation);

	FRotator NewSpringArmRot = m_SpringArm->GetComponentRotation();
	//NewSpringArmRot.Pitch += vec_CameraInput.Y;
	NewSpringArmRot.Pitch = FMath::Clamp(NewSpringArmRot.Pitch += vec_CameraInput.Y, -80.f, -15.f);

	m_SpringArm->SetWorldRotation(NewSpringArmRot);

	if (bClickMouseRight)
	{

	}


}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::MousePitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::MouseYaw);

	PlayerInputComponent->BindAction(TEXT("MouseRight"), IE_Pressed, this, &ACollider::InputRightClickPressed);
	PlayerInputComponent->BindAction(TEXT("MouseRight"), IE_Released, this, &ACollider::InputRightClickReleased);
	//PlayerInputComponent->BindAxis(TEXT("CameraRightClick"), this, &ACollider::MouseRightClick);

}

void ACollider::MoveForward(float value)
{
	FVector fForward = GetActorForwardVector();
	//AddMovementInput(value * fForward);	Pawn의 이동구성요소

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(fForward * value);
	}
}

void ACollider::MoveRight(float value)
{
	FVector fRight = GetActorRightVector();
	//AddMovementInput(value * fRight);

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(fRight * value);
	}
}

void ACollider::MouseYaw(float AxValue)
{
	vec_CameraInput.X = AxValue;
}

void ACollider::MousePitch(float AxValue)
{
	vec_CameraInput.Y = AxValue;
}

void ACollider::InputRightClickPressed()
{
	bClickMouseRight = true;

	UE_LOG(LogTemp, Warning, TEXT("Right Click True"));
}

void ACollider::InputRightClickReleased()
{
	bClickMouseRight = false;
	UE_LOG(LogTemp, Warning, TEXT("Right Click False"));
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return OurMovementComponent;
}


