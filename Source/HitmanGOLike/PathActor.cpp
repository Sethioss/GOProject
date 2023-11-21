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
	AddNeighbouringNodes();

	if (StartingNode)
	{
		APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		Pawn->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, PlayerLocation.Z));

		PlayerPawn = Cast<ACasaPlayer>(Pawn);

		UE_LOG(LogTemp, Warning, TEXT("Le start possede : %f noeud enfant"),NeighbouringNodes.Num());
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

void APathActor::TransferPlayerOwnership(APathActor& OriginTile)
{
	PlayerPawn = OriginTile.PlayerPawn;
	OriginTile.PlayerPawn = nullptr;
}

TArray<APathActor*> APathActor::ReachNeighbouringPath() {

	FHitResult North, South, East, West;
	TArray<FHitResult*> HitResults; HitResults.Add(&North); HitResults.Add(&South); HitResults.Add(&East); HitResults.Add(&West);
	FVector NodePosition = GetActorLocation();
	TArray<APathActor*> Neighbours;

	//Looking for Northern Object
	GetWorld()->LineTraceSingleByChannel(North, NodePosition, NodePosition + (50, 0, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Southern Object
	GetWorld()->LineTraceSingleByChannel(South, NodePosition, NodePosition + (-50, 0, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Eastern Object
	GetWorld()->LineTraceSingleByChannel(East, NodePosition, NodePosition + (0, 50, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Western Object
	GetWorld()->LineTraceSingleByChannel(West, NodePosition, NodePosition + (0, -50, 0),
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), NodePosition, South.ImpactPoint, FColor::Green, true, 50.f);

	//Process HitResult: Nearby object isNode -> add to NeighbouringNodes
	for (FHitResult* HitResult : HitResults) {
		if (HitResult->bBlockingHit) {
			APathActor* NearbyPath = Cast<APathActor>(HitResult->GetActor());

			//Si le voisin est un Node: alors on l'ajoute
			if (NearbyPath->GetIsNode()) {
				Neighbours.Add(NearbyPath);
			}
			
			//Si le voisin est un simple connecteur: alors on propage pour avoir acces aux Nodes suivant ce connecteur
			else {
				Neighbours.Append(NearbyPath->ReachNeighbouringPath());
			}
		}
	}

	return Neighbours;
}

void APathActor::AddNeighbouringNodes() {

	TArray<APathActor*> Neighbours = ReachNeighbouringPath();

	for (APathActor* Node : Neighbours) {
		if (Node != this) {
			(GetNeighbouringNodes()).AddUnique(Node);
		}
	}
}

