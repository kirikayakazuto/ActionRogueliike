// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "Components/TimelineComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	this->RootComponent = this->BaseMesh;

	this->LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	this->LidMesh->SetupAttachment(this->RootComponent);

	this->PileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PileMesh"));
	this->PileMesh->SetupAttachment(this->RootComponent);

	this->ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	this->ParticleSystemComp->SetupAttachment(this->PileMesh);
	this->ParticleSystemComp->SetAutoActivate(false);
	
	this->TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();

		
	FOnTimelineFloat OnTimelineFloat;
	FOnTimelineEvent OnTimelineEvent;
	
	OnTimelineFloat.BindUFunction(this, FName("OnTimelineTick"));
	OnTimelineEvent.BindUFunction(this, FName("OnTimelineEndEvent"));
	
	this->TimelineComp->AddInterpFloat(this->CurveFloat, OnTimelineFloat);
	this->TimelineComp->SetTimelineLength(0.5f);
	this->TimelineComp->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	this->TimelineComp->SetTimelineFinishedFunc(OnTimelineEvent);
	
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	this->TimelineComp->Play();
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASItemChest::OnTimelineTick(float value)
{
	this->LidMesh->SetRelativeRotation(FRotator(value, 0.f, 0.f));
}

void ASItemChest::OnTimelineEndEvent()
{
	this->ParticleSystemComp->Activate();
}

