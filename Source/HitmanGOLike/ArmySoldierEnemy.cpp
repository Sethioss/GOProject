// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmySoldierEnemy.h"
#include "GameManager.h"

AArmySoldierEnemy::AArmySoldierEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere")); //static permet d'executer la fonction qu'une fois
	Mesh->SetStaticMesh(meshFinder.Object);
}

void AArmySoldierEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AArmySoldierEnemy::NeutralTurn()
{
	Destination = GetDestination();
	if (Destination != nullptr)
	{
		MoveToDestination();
	}
}

APathActor* AArmySoldierEnemy::GetDestination()
{
	FHitResult HitResult;

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	APathActor* Dest = Cast<APathActor>(UGameManager::GetInstance()->GetPlayerNode());

	if (Dest != nullptr)
	{
		TArray<APathActor*> TempPath;
		TArray<int> Checkpoints;

		for (int i = 0; i < CurrentNode->ConnectorInfo.Num(); ++i)
		{
			GetBestPath(CurrentNode->ConnectorInfo[i].DestinationNode, Dest, TempPath, Checkpoints);
		}
	}
	
	//return GetDestFromPath(CurrentPath);
	return nullptr;
}

void AArmySoldierEnemy::GetBestPath(APathActor* Start, APathActor* End, TArray<APathActor*> TempPath, TArray<int> Checkpoints)
{
	Start->Visited = true;
	int NeighbouringNodesNum = Start->ConnectorInfo.Num();

	if (TempPath.Num() != 0)
	{

	}

	TempPath.Add(Start);
	bool FinalPathExists = BestPath.Num() > 0;

	//We know TempPath is too long, no need to continue, or it's a dead end. We invalidate the path either way
	if (FinalPathExists)
	{
		if (TempPath.Num() > BestPath.Num() || (IsDeadEnd(Start) && Start != End))
		{
			return;
		}
	}

	//Add current node to Checkpoints list if it has more than two branching paths, so we
	//know when to stop clearing TempPath later
	if (NeighbouringNodesNum > 2)
	{ 
		Checkpoints.Add(TempPath.Num()); 
	}

	for (int i = 0; i < NeighbouringNodesNum; ++i)
	{
		if (!Start->ConnectorInfo[i].DestinationNode->Visited)
		{
			GetBestPath(Start->ConnectorInfo[i].DestinationNode, End, TempPath, Checkpoints);
		}
	}

	//We reached the target, store this as BestPath and Unregister until checkpoint
	if (Start == End)
	{
		//Code to define if it's the best path
		TArray<APathActor*> FinalPath;

		if (BestPath.Num() < 1 || TempPath.Num() > BestPath.Num())
		{
			FinalPath = TempPath;
			BestPath = FinalPath;
			UnregisterVisitedNodesUntilLastCheckpoint(TempPath, Checkpoints);

			return;
		}

		//Decide on number of angles
	}
}

bool AArmySoldierEnemy::IsDeadEnd(APathActor* Node)
{
	for (int i = 0; i < Node->ConnectorInfo.Num(); ++i)
	{
		if (!Node->ConnectorInfo[i].DestinationNode->Visited) { return false; }
	}
	return true;
}

void AArmySoldierEnemy::UnregisterVisitedNodesUntilLastCheckpoint(TArray<APathActor*> TempPath, TArray<int> Checkpoints)
{
	int LastCheckpointPos = Checkpoints[Checkpoints.Num() - 1];

	for (int i = TempPath.Num(); i > LastCheckpointPos; ++i)
	{
		TempPath[i]->Visited = false;
		TempPath.Remove(TempPath[i]);
	}

	//Make the connector node revisitable
	TempPath[TempPath.Num() - 1]->Visited = false;
}

void AArmySoldierEnemy::MoveToDestination()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm the army soldier and I'm moving!"));
}

void AArmySoldierEnemy::InitFsm()
{
	Super::InitFsm();
}