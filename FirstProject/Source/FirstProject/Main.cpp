// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"

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
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = .2f;//���� ����




	fMaxHealth = 100.0f;
	fHealth = 65.0f;
	fMaxStamina = 150.0f;
	fStamina = 120.0f;
	nCoins = 0;

	fRunningSpeed = 250.0f;
	fSprintingSpeed = 950.0f;

	bShiftKeyDown = false;

	bLMBDown = false;

	//init Enums
	MovemoentStatus = EMovementStatus::EMS_NORMAL;
	StaminaStatus = EStaminaStatus::ESS_NORMAL;

	fStaminaDrainRate = 25.f;
	fMinSprintStamina = 50.0f;
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

	float DeltaStamina = fStaminaDrainRate * DeltaTime;
	
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_NORMAL:
		{
			if (bShiftKeyDown)
			{
				if (fStamina - DeltaStamina <= fMinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_BELOWMINIUM);
					fStamina -= DeltaStamina;
				}
				else
				{
					fStamina -= DeltaStamina;
				}

				SetMovementStatus(EMovementStatus::EMS_SPRINTING);
			}
			else
			{
				// Shift Up
				if (fStamina + DeltaStamina >= fMaxStamina)
				{
					fStamina = fMaxStamina;
				}
				else
				{
					fStamina += DeltaStamina;
				}

				SetMovementStatus(EMovementStatus::EMS_NORMAL);
			}

		} break;

		case EStaminaStatus::ESS_BELOWMINIUM:
		{
			if (bShiftKeyDown)
			{
				if (fStamina - DeltaStamina <= 0.0f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_EXHAUSTED);
					fStamina = 0;
					SetMovementStatus(EMovementStatus::EMS_NORMAL);
				}
				else
				{
					fStamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_SPRINTING);
				}
			}
			else
			{
				// Shift up
				if (fStamina + DeltaStamina >= fMinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_NORMAL);
					fStamina += DeltaStamina;
				}
				else
				{
					fStamina += DeltaStamina;
				}

				SetMovementStatus(EMovementStatus::EMS_NORMAL);
			}
		} break;

		case EStaminaStatus::ESS_EXHAUSTED:
		{
			if (bShiftKeyDown)
			{
				fStamina = 0.0f;
			}
			else
			{
				SetStaminaStatus(EStaminaStatus::ESS_EXHAUSTEDRECOVERING);
				fStamina += DeltaStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_NORMAL);
		} break;

		case EStaminaStatus::ESS_EXHAUSTEDRECOVERING:
		{
			if (fStamina + DeltaStamina >= fMinSprintStamina)\
			{
				SetStaminaStatus(EStaminaStatus::ESS_NORMAL);
				fStamina += DeltaStamina;
			}
			else
			{
				fStamina += DeltaStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_NORMAL);
		} break;

		default:;
	}
}


// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);


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
		
		UE_LOG(LogTemp, Warning, TEXT("MoveForward State : %f"), value);

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

void AMain::LMBDown()
{
	bLMBDown = true;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);

		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
}


void AMain::DecrementHealth(float amount)
{
	if (fHealth - amount <= 0.0f)
	{
		fHealth -= amount;
		Die();
	}
	else
	{
		fHealth -= amount;
	}
}

void AMain::IncrementCoins(int32 amount)
{
	nCoins += amount;
}

void AMain::Die()
{

}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovemoentStatus = Status;

	if (MovemoentStatus == EMovementStatus::EMS_SPRINTING)
	{
		GetCharacterMovement()->MaxWalkSpeed = fSprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = fRunningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::ShowPickupLocations()
{
	//for (int i = 0; i < PickupLocations.Num(); ++i)
	//{
	//	UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Green, 15.f, .25f);
	//}	

	/*for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 15.f, .25f);
	}*/

	for (FVector Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 15.f, .25f);
	}
}