// Fill out your copyright notice in the Description page of Project Settings.


#include "PathActor.h"
#include "Components/StaticMeshComponent.h"
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
}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();

	AddNeighbouringNodes();
	

	if (StartingNode)
	{
		//Joueur déplacé au point de départ
		APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		Pawn->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, PlayerLocation.Z));

		PlayerPawn = Cast<ACasaPlayer>(Pawn);

	}

	//Ajout des Noeuds voisins à NeighbouringNodes
	//AddNeighbouringNodes();
}

// Called every frame
void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Indique si le Joueur est sur un Node voisin du Node actuel
bool APathActor::IsPlayerOnNeighbouringNode()
{
	for (int i = 0; i < NeighbouringNodes.Num(); ++i)
	{
		if (NeighbouringNodes[i]->PlayerPawn != nullptr)
		{
			//Le Joueur est voisin du Node
			TransferPlayerOwnership(*NeighbouringNodes[i]);
			return true;
		}
	}
	return false;
}

//Indique si la Foreuse est sur un Node voisin du Node actuel
APathActor* APathActor::IsForeuseOnNeighbourinNode()
{
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		if (ConnectorInfo[i].DestinationNode->IsPlayerOnNeighbouringNode())
		{
			FVector TargetPos = GetActorLocation();
			FVector ForeusePos = ConnectorInfo[i].DestinationNode->GetActorLocation();
			if ((TargetPos.X == ForeusePos.X && ForeusePos.X == ConnectorInfo[i].DestinationNode->PlayerPawn->GetActorLocation().X) || (TargetPos.Y == ForeusePos.Y && ForeusePos.Y == ConnectorInfo[i].DestinationNode->PlayerPawn->GetActorLocation().Y))
			{
				return ConnectorInfo[i].DestinationNode;
			}
		}
	}
	/*for (FConnectorInfo* Node: ConnectorInfo) 
	{
		if (Node->IsPlayerOnNeighbouringNode()) 
		{
			FVector TargetPos = GetActorLocation();
			FVector ForeusePos = Node->GetActorLocation();
			if((TargetPos.X == ForeusePos.X && ForeusePos.X == Node->PlayerPawn->GetActorLocation().X)|| (TargetPos.Y == ForeusePos.Y && ForeusePos.Y == Node->PlayerPawn->GetActorLocation().Y))
			{ 
				return Node;
			}
		}
	}
	*/
	return nullptr;
}

void APathActor::TransferPlayerOwnership(APathActor& OriginTile)
{
	PlayerPawn = OriginTile.PlayerPawn;
	OriginTile.PlayerPawn = nullptr;
}

//Récupère tout les Nodes voisin de la Node
TArray<APathActor*> APathActor::ReachNeighbouringPath() 
{

	FHitResult North, South, East, West;
	TArray<FHitResult*> HitResults; HitResults.Add(&North); HitResults.Add(&South); HitResults.Add(&East); HitResults.Add(&West);
	FVector NodePosition = GetActorLocation();
	TArray<APathActor*> Neighbours;

	//Looking for Northern Object
	GetWorld()->LineTraceSingleByChannel(North, NodePosition+FVector(0,0,51), NodePosition + FVector(51, 0, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Southern Object
	GetWorld()->LineTraceSingleByChannel(South, NodePosition + FVector(0, 0, 51), NodePosition + FVector(-51, 0, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Eastern Object
	GetWorld()->LineTraceSingleByChannel(East, NodePosition + FVector(0, 0, 51), NodePosition + FVector(0, 51, 0),
		ECollisionChannel::ECC_GameTraceChannel1);
	//Looking for Western Object
	GetWorld()->LineTraceSingleByChannel(West, NodePosition + FVector(0, 0, 51), NodePosition + FVector(0, -51, 0),
		ECollisionChannel::ECC_GameTraceChannel1);

	//Process HitResult: Nearby object isNode -> add to NeighbouringNodes
	for (FHitResult* HitResult : HitResults) 
	{
		if (HitResult->bBlockingHit) 
		{
			APathActor* NearbyPath = Cast<APathActor>(HitResult->GetActor());
			if (NearbyPath) 
			{
				//Si le voisin est un Node: alors on l'ajoute
				if (NearbyPath->GetIsNode()) 
				{
					Neighbours.Add(NearbyPath);
				}

				//Si le voisin est un simple connecteur: alors on propage pour avoir acces aux Nodes suivant ce connecteur
				else 
				{
					Neighbours.Append(NearbyPath->ReachNeighbouringPath());
				}
			}
		}
	}

	return Neighbours;
}

//Ajoute les noeuds voisins à NeighbouringNodes
void APathActor::AddNeighbouringNodes() 
{
	TArray<APathActor*> Neighbours = ReachNeighbouringPath();

	for (APathActor* Node : Neighbours) 
	{
		if (Node != this) 
		{
			(GetNeighbouringNodes()).AddUnique(Node);
		}
	}
}

