// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmySoldierEnemy.h"
#include "Algo/Reverse.h"
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
	APathActor* Dest = Cast<APathActor>(UGameManager::GetInstance()->GetPlayerNode());

	if (Dest != nullptr)
	{
		TArray<APathActor*> TempPath;
		TempPath = AStarAlgorithm(GetCurrentNode(), Dest);
		if (TempPath.Num() > 2)
		{
			if (TempPath[1] == GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS))
			{
				BestPath = TempPath;
			}
			else 
			{
				TArray<APathActor*> TempPathTwo = AStarAlgorithm(GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS), Dest);
			}
		}
		else 
		{
			BestPath = TempPath;
			return;
		}
	}
}

APathActor* AArmySoldierEnemy::GetDestination()
{
	FHitResult HitResult;

	APathActor* Dest = Cast<APathActor>(UGameManager::GetInstance()->GetPlayerNode());

	if (Dest != nullptr)
	{
		BestPath = AStarAlgorithm(GetCurrentNode(), Dest);
	}

	return nullptr;
}

TArray<TArray<APathActor*>> AArmySoldierEnemy::GetBestPath(APathActor* Start, APathActor* End)
{
	TArray<TArray<APathActor*>> Dummy;
	/*Start->OriginNode->Visited = true;
	Start->DestinationNode->Visited = true;
	int NeighbouringNodesNum = Start->DestinationNode->ConnectorInfo.Num();

	//Set Angles Number
	if (TempPath.Num() > 0)
	{
		if (TempDir != Start->Direction)
		{
			AnglesNumber++;
		}
	}
	TempDir = Start->Direction;

	TempPath.Add(Start);
	bool FinalPathExists = BestPath.Num() > 0;

	//We know TempPath is too long, no need to continue, or it's a dead end. We invalidate the path either way
	if (FinalPathExists)
	{
		if (TempPath.Num() > BestPath.Num() || (IsDeadEnd(Start->DestinationNode) && Start->DestinationNode != End))
		{
			return;
		}
	}

	//Add current node to Checkpoints list if it has more than two branching paths, so we
	//know when to stop clearing TempPath later
	if (NeighbouringNodesNum > 2)
	{
		Checkpoints.Add(TempPath.Num() - 1);
	}

	//Recursively check next path
	for (int i = 0; i < NeighbouringNodesNum; ++i)
	{
		if (!Start->OriginNode->ConnectorInfo[i].DestinationNode->Visited)
		{
			GetBestPath(&Start->OriginNode->ConnectorInfo[i], End, TempPath, Checkpoints, TempDir, AnglesNumber);
		}
	}

	//We reached the target, store this as BestPath and Unregister until checkpoint
	if (Start->DestinationNode == End)
	{
		//Code to define if it's the best path
		TArray<FConnectorInfo*> FinalPath;

		if (BestPath.Num() < 1 || TempPath.Num() > BestPath.Num())
		{
			FinalPath = TempPath;
			BestPath = FinalPath;
			UnregisterVisitedNodesUntilLastCheckpoint(TempPath, Checkpoints);

			return;
		}

		//Decide on number of angles
		/*if (FinalPath.AnglesNum != BestPath.AnglesNum)
		{
			FinalPath = TempPath.AnglesNum < BestPath.AnglesNum ? TempPath : BestPath;
			BestPath = FinalPath;
			return;
		}
	}*/
	return Dummy;
}

bool AArmySoldierEnemy::IsDeadEnd(APathActor* Node)
{
	for (int i = 0; i < Node->ConnectorInfo.Num(); ++i)
	{
		if (!Node->ConnectorInfo[i].DestinationNode->Visited) { return false; }
	}
	return true;
}

TArray<APathActor*> AArmySoldierEnemy::AStarAlgorithm(APathActor* Start, APathActor* End)
{
	APathActor* Current = nullptr;

	TArray<APathActor*> OpenList;
	TArray<APathActor*> ClosedList;

	TArray<APathActor*> ListToSend;

	OpenList.Add(Start);

	while (!OpenList.IsEmpty())
	{

		APathActor* CurIT = OpenList[0];
		Current = CurIT;

		for (int i = 0; i < OpenList.Num(); ++i)
		{
			APathActor* SelectedNode = OpenList[i];

			if (SelectedNode->FScore <= Current->FScore) {
				Current = SelectedNode;
				CurIT = OpenList[i];
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("PATHFINDING: %s"), *Current->GetName());
		if (Current == End)
		{
			break;
		}

		ClosedList.Insert(Current, 0);
		OpenList.Remove(CurIT);

		for (int i = 0; i < Current->ConnectorInfo.Num(); ++i)
		{
			//If node has already been visited, don't add to openlist
			if (ClosedList.Contains(Current->ConnectorInfo[i].DestinationNode))
			{
				continue;
			}

			APathActor* Successor = Current->ConnectorInfo[i].DestinationNode;
			Current->ConnectorInfo[i].DestinationNode->FScore = ManhattanDistance(Current->ConnectorInfo[i].DestinationNode->GetActorLocation(), End->GetActorLocation());
			Successor->PathfindingParent = Current;

			OpenList.Insert(Current->ConnectorInfo[i].DestinationNode, 0);

		}
	}

	while (Current != nullptr)
	{
		ListToSend.Add(Current);
		Current = Current->PathfindingParent;
	}
	//Make path list
	Algo::Reverse(ListToSend);
	return ListToSend;
}

float AArmySoldierEnemy::ManhattanDistance(FVector A, FVector B)
{
	float X = A.X - B.X;
	float Y = A.Y - B.Y;

	float h = FMath::Abs(X) + FMath::Abs(Y);
	return h;
}

void AArmySoldierEnemy::MoveToDestination()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm the army soldier and I'm moving!"));
}

void AArmySoldierEnemy::InitFsm()
{
	Super::InitFsm();
}