// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(GetRootComponent());
	m_CameraBoom->TargetArmLength = 400.f;	// 카메라와 플레이어 거리
	m_CameraBoom->bUsePawnControlRotation = true;	// 컨트롤러 기반 팔 회전

	GetCapsuleComponent()->SetCapsuleSize(48.f, 80.f);

	// Create Folow Camera
	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);	

	//m_FollowCamera->SetRelativeLocation(FVector(-100, 0, 300));
	//m_FollowCamera->SetRelativeRotation(FRotator(-45, 0, 0));

	m_FollowCamera->bUsePawnControlRotation = false;

	// Set Our TurnRate Trun
	m_fBaseTurnRate = 65.f;
	m_fBaseLookUpRate = 65.f;

	// 카메라에 영향을 미친다
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;  // 입력방향으로 캐릭터 이동
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);	// 캐릭터 회전 속도
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .2f;//공중 제어
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("CameraPitch", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

	/*PlayerInputComponent->BindAction(TEXT("MouseRight"), IE_Pressed, this, &AMain::InputRightClickPressed);
	PlayerInputComponent->BindAction(TEXT("MouseRight"), IE_Released, this, &AMain::InputRightClickReleased);*/
}

void AMain::MoveForward(float value)
{
	if (Controller != nullptr && value != 0.f)
	{
		// 캐릭터의 앞방향 위치를 찾는다.
		const FRotator Rot = Controller->GetControlRotation();		// 절대축 기준 회전값
		const FRotator YawRotation(0.f, Rot.Yaw, 0.f);
		// 순 방향 FRotMatrix 시스템으로 인하여 회전을 얻고 그 회전으로부터 전방 벡터를 구한다.
		
		const FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	
		// 회전값을 행렬로 변환 -> 절대축 기준 회전값 가져오기 : X축 기준으로 얼마나 회전되었는지 계산
		AddMovementInput(Dir, value);
		
		//FVector fForward = GetActorForwardVector();
		//AddMovementInput(fForward, value);
	}
}

void AMain::MoveRight(float value)
{
	if (Controller != nullptr && value != 0.f)
	{
		// 캐릭터의 앞방향 위치를 찾는다.
		const FRotator Rot = Controller->GetControlRotation();		// 절대축 기준 회전값
		const FRotator YawRotation(0.f, Rot.Yaw, 0.f);
		// 순 방향 FRotMatrix 시스템으로 인하여 회전을 얻고 그 회전으로부터 전방 벡터를 구한다.

		const FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		
		// 회전값을 행렬로 변환 -> 절대축 기준 회전값 가져오기 : Y축 기준으로 얼마나 회전되었는지 계산
		AddMovementInput(Dir, value);

		/*FVector fRight = GetActorRightVector();
		AddMovementInput(fRight, value);*/
	}
}


// 입력을 받아 축에서 컨트롤러를 구체적으로 회전하는 기능
void AMain::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * m_fBaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * m_fBaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
