// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FIRSTPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	/// <summary>
	/// �⺻ ��� �ø��� 
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// /** ���𰡰� ��ü ������Ʈ�� �� �� ȣ�� */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// <summary>
	/// /** ���𰡰� ��ü ������Ʈ�� ���� �� ȣ�� */
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
