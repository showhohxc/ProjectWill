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
		
		/*  ���� �ݸ��� üũ�� �ϴ� ���Ͱ� ��ħ �̺�Ʈ�� �߻��Ҷ����� �������� �Դ� Ŭ�����ε� �������� ���� �� �ִ� Ŭ������ AProjectile Ŭ������� �������� ��, 
			���� ���� �ڵ�ó�� ������ �˻縦 ���� �ʴ´ٸ�, ���Ͱ� �ƹ� ��ü���� ��ĥ ������ �������� �Ծ���� ���̴�. 

			�ٷ� ���� ���� �ڵ�ó�� ĳ������ �����ϸ� �ȴ�. ���� �ݸ����� ����� ���Ͱ� AMain Ŭ������ �ƴ϶��
			ĳ���ÿ� ������ ���̰� main ������ ���� nullptr�� �Ǳ� ������ if�� ������ �������� ���ؼ� Damage Process�� ������� �ʴ´�.
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