// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "ActorMeshComponents")
	UStaticMeshComponent* StaticMeshPerty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Var")
	FVector InitialLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Var")
	FVector PlacedLoacation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Var")
	FVector WorldOrgin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Var")
	FVector InitialDir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Var")
	bool bShowFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Floater Var")
	bool bInitFloaterLocal;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Var")
	FVector InitForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Var")
	FVector InitTorqueRadians;
	/*
	EditInstanceOnly	: 레벨 에디터 디테일 창에서만 변수가 노출, 편집가능 
	VisibleInstanceOnly : 레벨 에디터 디테일 창에서만 변수가 노출, 편비 불가능

	EditDefaultsOnly	: 블루프린트 에디터의 디테일 창에서만 해당 변수 노출, 편집가능
	VisibleDefaultsOnly : 블루프린트 에디터의 디테일 창에서만 해당 변수 노출, 편집불가능
	*/

private:
	float fRunningTime;
	float fBaseZLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
