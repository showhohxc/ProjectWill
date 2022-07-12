// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	/*���� �ִϸ��̼ǿ� �ռ� ����Ǵ� ���� ���� �ܰ迡�� ���� ���ŵȴٸ� �� �ڿ� ����Ǵ� �ִϸ��̼� ���������� �� �̻� ��ȿ���� ���� �� ��ü�� �����ϴ� ���°� �߻��� ���̴�.
	�׷��� �̷��� ���¸� �����ϱ� ���� �ִϸ��̼��� �����ϱ� �� ���� ��ȿ���� �˻��ؾ� �Ѵ�. �̸� �˻��ϴ� ����� TryGetPawnOwner()��.*/
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();

		if (m_Pawn)
		{
			m_Enemy = Cast<AEnemy>(m_Pawn);
		}
	}
}


void UEnemyAnimInstance::UpdateAnimationProperties()
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

		//UE_LOG(LogTemp, Warning, TEXT("Enemy Speed : %f "), m_fMoveMentSpeed);

		if (m_Enemy == nullptr)
		{
			m_Enemy = Cast<AEnemy>(m_Pawn);
		}

		//UE_LOG(LogTemp, Warning, TEXT("JUMP State : %s"), m_bIsInAir ? TEXT("TRUE") : TEXT("FALSE") );
	}
}