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
	m_CameraBoom->TargetArmLength = 400.f;	// ī�޶�� �÷��̾� �Ÿ�
	m_CameraBoom->bUsePawnControlRotation = true;	// ��Ʈ�ѷ� ��� �� ȸ��

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

	// ī�޶� ������ ��ģ��
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;  // �Է¹������� ĳ���� �̵�
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);	// ĳ���� ȸ�� �ӵ�
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .2f;//���� ����
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
		// ĳ������ �չ��� ��ġ�� ã�´�.
		const FRotator Rot = Controller->GetControlRotation();		// ������ ���� ȸ����
		const FRotator YawRotation(0.f, Rot.Yaw, 0.f);
		// �� ���� FRotMatrix �ý������� ���Ͽ� ȸ���� ��� �� ȸ�����κ��� ���� ���͸� ���Ѵ�.
		
		const FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	
		// ȸ������ ��ķ� ��ȯ -> ������ ���� ȸ���� �������� : X�� �������� �󸶳� ȸ���Ǿ����� ���
		AddMovementInput(Dir, value);
		
		//FVector fForward = GetActorForwardVector();
		//AddMovementInput(fForward, value);
	}
}

void AMain::MoveRight(float value)
{
	if (Controller != nullptr && value != 0.f)
	{
		// ĳ������ �չ��� ��ġ�� ã�´�.
		const FRotator Rot = Controller->GetControlRotation();		// ������ ���� ȸ����
		const FRotator YawRotation(0.f, Rot.Yaw, 0.f);
		// �� ���� FRotMatrix �ý������� ���Ͽ� ȸ���� ��� �� ȸ�����κ��� ���� ���͸� ���Ѵ�.

		const FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		
		// ȸ������ ��ķ� ��ȯ -> ������ ���� ȸ���� �������� : Y�� �������� �󸶳� ȸ���Ǿ����� ���
		AddMovementInput(Dir, value);

		/*FVector fRight = GetActorRightVector();
		AddMovementInput(fRight, value);*/
	}
}


// �Է��� �޾� �࿡�� ��Ʈ�ѷ��� ��ü������ ȸ���ϴ� ���
void AMain::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * m_fBaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * m_fBaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
