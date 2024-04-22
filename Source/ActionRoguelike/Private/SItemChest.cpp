// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	this->RootComponent = this->BaseMesh;

	this->LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	this->LidMesh->SetupAttachment(this->RootComponent);

	this->TargetPitch = 110.f;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	this->LidMesh->SetRelativeRotation(FRotator(this->TargetPitch, 0.f, 0.f));
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

