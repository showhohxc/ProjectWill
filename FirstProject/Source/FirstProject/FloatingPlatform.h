// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere)
		FVector m_vStartPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
		FVector m_vEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float m_fInterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float m_fInterTime;

	FTimerHandle m_InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool m_bInterPing ;

	float m_fDistance;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterPing();
	void SwapVectors(FVector& VecOne, FVector& VecTwo);
};
