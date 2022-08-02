// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EMS_PICKUP UMETA(DisplayName = "Pickup"),
	EMS_EQUIPPED UMETA(DisplaName = "Equip"),

	EMS_MAX UMETA(DisplaName = "max"),
};

UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	EWeaponState m_WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	bool bWeaponParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;		// 이전에 선언해서 Class 선언이 필요없다

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* m_SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* m_CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage;


protected:
	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMain* Char);

	FORCEINLINE void SetWeaponState(EWeaponState weaponstate) { m_WeaponState = weaponstate; }
	FORCEINLINE EWeaponState GetWeaponState() { return m_WeaponState; }

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	void ActivateCollison();

	UFUNCTION(BlueprintCallable)
	void DeActivateCollison();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
};
