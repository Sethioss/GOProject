// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "PathActor.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation du component staticmesh
	StaticMeshComponent =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visuel"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	StaticMeshComponent->SetStaticMesh(meshFinder.Object);

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	CurrentNode = SnapToGrid();

	CurrentNode->HasObjectOnIt = true;
	CurrentNode->IsObstacle = true;
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Casse le Mur, permet de le traverser
void AWall::Break() 
{
	if (!IsBroken)
	{
		IsBroken = true;
		//static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //Modification du Mesh
		//StaticMeshComponent->SetStaticMesh(meshFinder.Object);

		//TEMPORARY - SAFELY REMOVE FROM BOARD
		SetActorLocation(FVector(10000, 10000, 10000));
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
	}
}

APathActor* AWall::SnapToGrid(FVector offset)
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -(GetActorUpVector() * 1000), ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - (GetActorUpVector() * 1000), FColor::Green, true, 50.f);

	if (HitResult.bBlockingHit)
	{

		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			CurrentNode = Path;

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z));
			CurrentNode->HasObjectOnIt = true;
			CurrentNode->IsObstacle = true;
		}
	}

	return CurrentNode;
}
