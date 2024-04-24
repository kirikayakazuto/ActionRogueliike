// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	this->RootComponent = this->SphereComp;

	// this->SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// this->SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	this->SphereComp->SetCollisionProfileName("Projectile");
	
	this->EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	this->EffectComp->SetupAttachment(this->RootComponent);

	this->HitParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticleComp"));
	this->HitParticleComp->SetupAttachment(this->RootComponent);
	this->HitParticleComp->SetAutoActivate(false);
	
	this->MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	this->MovementComp->InitialSpeed = 2000.0f;
	this->MovementComp->bRotationFollowsVelocity = true;
	this->MovementComp->bInitialVelocityInLocalSpace = true;

	this->SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnHit);
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->SphereComp->IgnoreActorWhenMoving(this->GetInstigator(), true);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == this->GetInstigator()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("call OnHit"));
	this->HitParticleComp->Activate();
	// this->Destroy();
	this->GetWorldTimerManager().SetTimer(this->TimerHandle_Destroy, this, &ASMagicProjectile::Destroy_TimeElapsed, 0.3f);
	
}

void ASMagicProjectile::Destroy_TimeElapsed()
{
	this->Destroy();
}

