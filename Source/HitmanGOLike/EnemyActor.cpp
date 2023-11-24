// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentNode = GetCurrentNode();
	if (CurrentNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found the node!"));
	}
	
}

void AEnemyActor::PatrolStep()
{
	
}

void AEnemyActor::AlertedStart()
{
}

void AEnemyActor::AlertedStep()
{
}

void AEnemyActor::AlertedEnd()
{
}

void AEnemyActor::Attack()
{
}

APathActor* AEnemyActor::GetCurrentNode()
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -GetActorUpVector() * 1000, ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - (GetActorUpVector() * 1000), FColor::Green, true, 50.f);

	if (HitResult.bBlockingHit)
	{

		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			CurrentNode = Path;
			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, GetActorLocation().Z));
		}
	}

	return CurrentNode;
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

