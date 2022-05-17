// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	/*���� �ִϸ��̼ǿ� �ռ� ����Ǵ� ���� ���� �ܰ迡�� ���� ���ŵȴٸ� �� �ڿ� ����Ǵ� �ִϸ��̼� ���������� �� �̻� ��ȿ���� ���� �� ��ü�� �����ϴ� ���°� �߻��� ���̴�.
	�׷��� �̷��� ���¸� �����ϱ� ���� �ִϸ��̼��� �����ϱ� �� ���� ��ȿ���� �˻��ؾ� �Ѵ�. �̸� �˻��ϴ� ����� TryGetPawnOwner()��.*/
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::BeginPlayAnimationProperties()
{
	TCHAR* str = TEXT("Blueprint'/Game/BluePrint/CritterGameMode_BP.CritterGameMode_BP'");
	UBlueprintGeneratedClass* LoadBp = LoadObject<UBlueprintGeneratedClass>(nullptr, str, nullptr); 
//	AMain* MainBp = Cast<AMain>(LoadBp);
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
	}

	if (m_Pawn)
	{
		FVector Speed = m_Pawn->GetVelocity();
		FVector LaterlaSpeed = FVector(Speed.X, Speed.Y, 0.f);
		m_fMoveMentSpeed = LaterlaSpeed.Size();

		m_bIsInAir = m_Pawn->GetMovementComponent()->IsFalling();

		UE_LOG(LogTemp, Warning, TEXT("JUMP State : %s"), m_bIsInAir ? TEXT("TRUE") : TEXT("FALSE") );
	}
}
 