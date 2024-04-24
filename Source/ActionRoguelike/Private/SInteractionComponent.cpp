// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = this->GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;

	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	
	
	FHitResult Hit;
	const FVector End = EyeLocation + (EyeRotation.Vector() * 500.0f);
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	if (AActor* HitActor = Hit.GetActor())
	{
		if (HitActor->Implements<USGameplayInterface>())
		{
			ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(Owner));
		}
	}

	const FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(this->GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

