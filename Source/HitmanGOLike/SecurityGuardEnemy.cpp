// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityGuardEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Called when the game starts or when spawned
void ASecurityGuardEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentNode)
	{
		PatrolStep();
	}

}

ASecurityGuardEnemy::ASecurityGuardEnemy()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	mesh->SetStaticMesh(meshFinder.Object);

	RootComponent = mesh;

	State = EEnemyState::NEUTRAL;

}

void ASecurityGuardEnemy::PatrolStep() 
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, CurrentNode->GetActorLocation() + FVector(0, 0, 51), GetActorForwardVector() * CurrentNode->GetActorLocation() + FVector(0, 51, 0), ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), CurrentNode->GetActorLocation() + FVector(0, 0, 51), GetActorForwardVector() * CurrentNode->GetActorLocation() + FVector(0, 51, 0), FColor::Red, true, 50.f);

	if (HitResult.bBlockingHit)
	{

		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, GetActorLocation().Z));
		}
	}
}

void ASecurityGuardEnemy::AlertedStart()
{
}

void ASecurityGuardEnemy::AlertedStep()
{
}

void ASecurityGuardEnemy::AlertedEnd()
{
}

void ASecurityGuardEnemy::Attack()
{
}
