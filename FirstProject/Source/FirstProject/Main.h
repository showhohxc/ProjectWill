// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	/*
		Camera Boom positioning the camera
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* m_CameraBoom;


	/* FollowCamera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* m_FollowCamera;

	// Base Turn Rated to Scale Turning Functions for the Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float m_fBaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float m_fBaseLookUpRate;


	// Character Stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Player Stats")
	float fMaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float fHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float fMaxStanmina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float fStanmina; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	int nCoins;






protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);

	/*
		Called via input to turn at a given rate
		* @param Rate This ia a normalized rate, i.e, 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float rate);

	// 주어진 속도의 위아래를 찾는 사용 - 정규화된 비율
	void LookUpAtRate(float rate);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }

};
