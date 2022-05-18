// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
	m_pTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);
	m_pTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloor::OnOverlapEnd);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin."));
}

void AFloor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End."));
}