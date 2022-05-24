// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_pTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = m_pTriggerBox;

	m_pTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pTriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	m_pTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_pTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_pTriggerBox->SetBoxExtent(FVector(62.0f, 62.0f, 32.0f));

	m_pFloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	m_pFloorSwitch->SetupAttachment(GetRootComponent());

	m_pDoor= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	m_pDoor->SetupAttachment(GetRootComponent());
	
	m_fSwitchTime = 2.0f;
	m_bChacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
	m_pTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);
	m_pTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloor::OnOverlapEnd);

	vec_InitDoorLocation = m_pDoor->GetComponentLocation();
	vec_InitFloorLocation = m_pFloorSwitch->GetComponentLocation();
	UE_LOG(LogTemp, Warning, TEXT("vec_InitDoorLocation %f %f %f"), vec_InitDoorLocation.X, vec_InitDoorLocation.Y, vec_InitDoorLocation.Z);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_bChacterOnSwitch != true)
		m_bChacterOnSwitch = true;

	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin."));
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_bChacterOnSwitch)
		m_bChacterOnSwitch = false;

	UE_LOG(LogTemp, Warning, TEXT("Overlap End."));
	GetWorldTimerManager().SetTimer(TimerSwitchHandle, this, &AFloor::CloseDoor, m_fSwitchTime);
}
	
void AFloor::UpdateDoorLocation(float z)
{
	FVector NewLocation = vec_InitDoorLocation;
	NewLocation.Z += z;
	m_pDoor->SetWorldLocation(NewLocation);
	//UE_LOG(LogTemp, Warning, TEXT("vec_InitDoorLocation %f %f %f"),  NewLocation.X, NewLocation.Y, NewLocation.Z);
}

void AFloor::UpdateFloorSwitchLocation(float z)
{
	FVector NewLocation = vec_InitFloorLocation;
	NewLocation.Z += z;
	m_pFloorSwitch->SetWorldLocation(NewLocation);
	//UE_LOG(LogTemp, Warning, TEXT("vec_InitFloorLocation %f %f %f"), NewLocation.X, NewLocation.Y, NewLocation.Z);
}

void AFloor::CloseDoor()
{
	if (m_bChacterOnSwitch != true)
	{
		LowerDoor();
		RaiseFloorSwitch();
	}
}