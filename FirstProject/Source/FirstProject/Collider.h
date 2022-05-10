// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class FIRSTPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* m_MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USphereComponent* m_SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UCameraComponent* m_Camera;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USpringArmComponent* m_SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UColliderMovementComponent* OurMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UParticleSystemComponent* OurParticleSystem;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Getter Setter
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return m_MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { Mesh = m_MeshComponent; }

	FORCEINLINE USphereComponent* GetSphereComponent() { return m_SphereComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* Sphere) { Sphere = m_SphereComponent; }

	FORCEINLINE UCameraComponent* GetCameraComponent() { return m_Camera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* Camera) { Camera = m_Camera; }

	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return m_SpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* SpringArm) { SpringArm = m_SpringArm; }

private:
	void MoveForward(float value);
	void MoveRight(float value);

	void MousePitch(float AxValue);
	void MouseYaw(float AxValue);

	void InputRightClickPressed();
	void InputRightClickReleased();

	FVector2D vec_CameraInput;
	bool bClickMouseRight;
};
