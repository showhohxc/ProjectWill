// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"

AExplosive::AExplosive()
{
	fDamage = 15.f;
}


void AExplosive:: OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Overlap Explosive Begin."));

	if (OtherActor)
	{
		AMain* main = Cast<AMain>(OtherActor);
		
		/*  만약 콜리전 체크를 하는 액터가 겹침 이벤트가 발생할때마다 데미지를 입는 클래스인데 데미지를 입힐 수 있는 클래스가 AProjectile 클래스라고 가정했을 때, 
			위의 예시 코드처럼 별도의 검사를 하지 않는다면, 액터가 아무 물체에나 스칠 때마다 데미지를 입어버릴 것이다. 

			바로 위의 예시 코드처럼 캐스팅을 진행하면 된다. 만약 콜리전에 검출된 액터가 AMain 클래스가 아니라면
			캐스팅에 실패할 것이고 main 변수의 값을 nullptr이 되기 때문에 if문 안으로 진행하지 못해서 Damage Process가 진행되지 않는다.
			*/

		if (main)
		{
			main->DecrementHealth(fDamage);

			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Overlap Explosive End."));
}