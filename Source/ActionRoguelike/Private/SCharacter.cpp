// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	this->SpringArmComp->bUsePawnControlRotation = true;
	this->SpringArmComp->SetupAttachment(this->RootComponent);
	
	this->CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	this->CameraComp->SetupAttachment(this->SpringArmComp);

	this->InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));
	

	GetCharacterMovement()->bOrientRotationToMovement = true;
	this->bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float value)
{
	auto ControlRot = this->GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	this->AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	auto ControlRot = this->GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	const auto RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	this->AddMovementInput(RightVector, value);
}


void ASCharacter::PrimaryAttack()
{
	this->PlayAnimMontage(this->AttackAnim);

	this->GetWorldTimerManager().SetTimer(this->TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryInteract()
{
	this->InteractionComp->PrimaryInteract();
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);

	FHitResult HitResult;

	FVector CameraLocation;
	FRotator CameraRotation;
	this->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector EndLocation = CameraLocation + CameraRotation.Vector() * 300
	FCollisionQueryParams Params(FName(TEXT("RayTrace")), true, this->GetController()->GetPawn());
	this->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params);
	// DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Red, false, 5.0f);
	const auto HandLocation = this->GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM;
	if (HitResult.bBlockingHit)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitResult.ImpactPoint);
		SpawnTM = FTransform(LookAtRotation, HandLocation);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("FindLookAtRotation"));
	} else
	{
		SpawnTM = FTransform(GetControlRotation(), HandLocation);
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnParams.Instigator = this;
	this->GetWorld()->SpawnActor<AActor>(this->ProjectileClass, SpawnTM, SpawnParams);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

