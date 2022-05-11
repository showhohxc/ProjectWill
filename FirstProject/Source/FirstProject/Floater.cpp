 // Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshPerty = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	InitialLocation = FVector(300, 300, 300);
	PlacedLoacation = FVector(0);
	WorldOrgin = FVector(0);
	InitialDir = FVector(0);

	bInitFloaterLocal = false;
	bShowFloat = false;

	InitForce = FVector(20000, 0, 0);
	InitTorqueRadians = FVector(20000, 0, 0);

	fRunningTime = 0.0f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	
	/*float InitialX = FMath::FRandRange(-500, 500);
	float InitialY = FMath::FRandRange(-500, 500);
	float InitialZ = FMath::FRandRange(0, 500);

	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;*/

	PlacedLoacation = GetActorLocation();
	
	if(bInitFloaterLocal)
		SetActorLocation(InitialLocation);

	/*FHitResult fHitResult;
	FVector LocalOffset = FVector(200, 0, 0);
	AddActorWorldOffset(LocalOffset, true, &fHitResult);*/

	StaticMeshPerty->AddForce(InitForce);
	StaticMeshPerty->AddTorqueInRadians(InitTorqueRadians);
	fBaseZLocation = 0;
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShowFloat)
	{
		/*FHitResult fHitResult;
		AddActorLocalOffset(InitialDir, true, &fHitResult);
		FVector HitLocation = fHitResult.Location;*/
		/*UE_LOG(LogTemp, Warning, TEXT("Hit Location : X - %f, Y - %f, Z - %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);*/
		FVector NewLocation = GetActorLocation();

		NewLocation.Z = PlacedLoacation.Z + 100 * FMath::Sin(fRunningTime);

		SetActorLocation(NewLocation);
		fRunningTime += DeltaTime;
	}
}

