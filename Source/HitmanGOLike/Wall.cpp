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

	// creation du component audio
	AudioComponent =
		CreateDefaultSubobject<UAudioComponent>(TEXT("Son"));

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 1), GetActorLocation() + FVector(0, 0, -1) * 1000,
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		APathActor* Node = Cast<APathActor>(HitResult.GetActor());
		if (Node)
		{
			CurrentNode = Node;
			SetActorLocation(FVector(Node->GetActorLocation().X, Node->GetActorLocation().Y, 50));
		}
	}
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
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
	}
}
