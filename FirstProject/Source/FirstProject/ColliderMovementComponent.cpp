// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 모든 것이 여전히 유효하고, 우리가 움직일 수 있는지 확인하세요.
	// PawnOwner : Pawn의 움직임을 업데이트하는데 사용되는 컴포넌트
	// UpdatedComponent : 이 컴포넌트를 소유하고 있는 폰
	// ShouldSkipUpdate: UPawnMovementComponent의 부모 클래스인 UMovementComponent 클래스에 속하는 변수로 이 무브먼트 컴포넌트가 이동시키고 업데이트 해야할 컴포넌트
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return;

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f);
	// ConsumeInputVector 는 미동 입력을 저장하는 데 사용할 내장 변수 값을 보고 및 소거합니다. 이동된 컴포넌트가 이동할 수 없거나 렌더링되지 않은 경우인지를 판별하여 알려주는 함수

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		// SafeMoveUpdatedComponent 는 언리얼 엔진 피직스를 사용하여 입체 장애물을 피해 Pawn Movement Component 를 이동시킵니다. 대기중인 입력을 반환하고 다시 0으로 설정하는 함수

		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);	// 부드럽게 미끄러진다 Good**
			/* SlideAlongSurface 는 이동하다가 충돌 발생시 그냥 제자리에 멈춰 벽이나 경사로에 "달라붙기" 보다는, 
				그 표면을 타고 부드럽게 미끄러지도록 하는 데 관련된 물리 계산 처리를 합니다.
				언리얼 엔진 피직스를 이용해서 입체 장애물을 피해서 폰 무브먼트 컴포넌트를 이동시키는 함수
			*/

			UE_LOG(LogTemp, Warning, TEXT("Valid Blocking Hit"));
		}
	}
}