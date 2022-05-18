// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	// Native Init Animation
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationsProperties)
	void BeginPlayAnimationProperties();

	//UFUNCTION(BlueprintPure, Category = "Charater Speed")
	//	float GetCharacterSpeed(float fValue)
	//{
	//	return m_fMoveMentSpeed + fValue;
	//}

	UFUNCTION(BlueprintCallable, Category = AnimationsProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float m_fMoveMentSpeed;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool m_bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* m_Pawn;



};
