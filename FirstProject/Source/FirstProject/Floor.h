// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
		class UBoxComponent* m_pTriggerBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
		class UStaticMeshComponent* m_pFloorSwitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
		class UStaticMeshComponent* m_pDoor;

	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
		FVector vec_InitDoorLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
		FVector vec_InitFloorLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
		float m_fSwitchTime;

	FTimerHandle TimerSwitchHandle;
	bool		 m_bChacterOnSwitch;

	UFUNCTION()
	void CloseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
		void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
		void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
		void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
		void UpdateFloorSwitchLocation(float z);

};
