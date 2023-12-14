// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoverComponent::UMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UMoverComponent::Setup(FVector DestinationPos)
{
	StartPosition = GetOwner()->GetActorLocation();
	DestinationPosition = DestinationPos;
}

bool UMoverComponent::MoveTo(float TimeItTakes)
{
	if (!ClockStarted)
	{
		//Clock = GetWorld()->GetTimeSeconds();
		ClockStarted = true;
	}
	else
	{
		GetOwner()->SetActorLocation(FMath::Lerp(StartPosition, DestinationPosition, MovementCurve->GetFloatValue(Clock) / TimeItTakes));
		UE_LOG(LogTemp, Warning, TEXT("%f"), MovementCurve->GetFloatValue(Clock) / TimeItTakes);
		Clock += GetWorld()->GetDeltaSeconds();
	}

	return (Clock > TimeItTakes);

}

