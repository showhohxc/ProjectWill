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
	/// 기본 모양 컬리전 
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
		class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
		class UParticleSystem* OverlapParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
		class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperty")
	bool bRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperty")
	float fRotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// /** 무언가가 구체 컴포넌트에 들어설 때 호출 */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// <summary>
	/// /** 무언가가 구체 컴포넌트에 나갈 때 호출 */
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
