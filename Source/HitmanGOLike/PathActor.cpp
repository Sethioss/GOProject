// Fill out your copyright notice in the Description page of Project Settings.


#include "PathActor.h"
#include "Components/StaticMeshComponent.h"
#include "CasaPlayer.h"
#include "Engine/World.h"

// Sets default values
APathActor::APathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridPlane"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Plane.Plane")); //static permet d'executer la fonction qu'une fois
	PlaneMesh->SetStaticMesh(meshFinder.Object);

	RootComponent = PlaneMesh;
}

void APathActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (GetIsNode())
	{
		UE_LOG(LogTemp, Warning, TEXT("The node is here"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Vincent aime pas les yoshis noirs :'("));
	}
}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartingNode)
	{
		APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		Pawn->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, PlayerLocation.Z));

		PlayerPawn = Cast<ACasaPlayer>(Pawn);
	}
}

// Called every frame
void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APathActor::IsPlayerOnNeighbouringNode()
{
	for (int i = 0; i < NeighbouringNodes.Num(); ++i)
	{
		if (NeighbouringNodes[i]->PlayerPawn != nullptr)
		{
			TransferPlayerOwnership(*NeighbouringNodes[i]);
			return true;
		}
	}
	return false;
}

bool APathActor::IsConnectedNode(APathActor* A, APathActor* B)
{
	for (int i = 0; i < A->NeighbouringNodes.Num(); ++i)
	{
		if (A->NeighbouringNodes[i] == B) 
		{
			return true;
		}
	}
	return false;
}

void APathActor::TransferPlayerOwnership(APathActor& OriginTile) 
{
	PlayerPawn = OriginTile.PlayerPawn;
	OriginTile.PlayerPawn = nullptr;
}