// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ��� ���� ������ ��ȿ�ϰ�, �츮�� ������ �� �ִ��� Ȯ���ϼ���.
	// PawnOwner : Pawn�� �������� ������Ʈ�ϴµ� ���Ǵ� ������Ʈ
	// UpdatedComponent : �� ������Ʈ�� �����ϰ� �ִ� ��
	// ShouldSkipUpdate: UPawnMovementComponent�� �θ� Ŭ������ UMovementComponent Ŭ������ ���ϴ� ������ �� �����Ʈ ������Ʈ�� �̵���Ű�� ������Ʈ �ؾ��� ������Ʈ
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return;

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f);
	// ConsumeInputVector �� �̵� �Է��� �����ϴ� �� ����� ���� ���� ���� ���� �� �Ұ��մϴ�. �̵��� ������Ʈ�� �̵��� �� ���ų� ���������� ���� ��������� �Ǻ��Ͽ� �˷��ִ� �Լ�

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		// SafeMoveUpdatedComponent �� �𸮾� ���� �������� ����Ͽ� ��ü ��ֹ��� ���� Pawn Movement Component �� �̵���ŵ�ϴ�. ������� �Է��� ��ȯ�ϰ� �ٽ� 0���� �����ϴ� �Լ�

		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);	// �ε巴�� �̲������� Good**
			/* SlideAlongSurface �� �̵��ϴٰ� �浹 �߻��� �׳� ���ڸ��� ���� ���̳� ���ο� "�޶�ٱ�" ���ٴ�, 
				�� ǥ���� Ÿ�� �ε巴�� �̲��������� �ϴ� �� ���õ� ���� ��� ó���� �մϴ�.
				�𸮾� ���� �������� �̿��ؼ� ��ü ��ֹ��� ���ؼ� �� �����Ʈ ������Ʈ�� �̵���Ű�� �Լ�
			*/

			UE_LOG(LogTemp, Warning, TEXT("Valid Blocking Hit"));
		}
	}
}