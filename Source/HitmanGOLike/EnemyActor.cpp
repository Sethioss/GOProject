// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Engine/World.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	Mesh->SetStaticMesh(meshFinder.Object);

	RootComponent = Mesh;

	State = EEnemyState::NEUTRAL;
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentNode = SnapToGrid();
	
}

void AEnemyActor::Update()
{
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

void AEnemyActor::MoveToDestination()
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

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z + (ActorBounds.GetExtent().Z)));
		}
	}

	return CurrentNode;
}

APathActor* AEnemyActor::SnapToGrid(FVector offset)
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

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z + (ActorBounds.GetSize().Z / 2)));
		}
	}

	return CurrentNode;
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

