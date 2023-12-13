// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmySoldierEnemy.h"
#include "Algo/Reverse.h"
#include "GameManager.h"
#include "Containers/Array.h"

UE_DISABLE_OPTIMIZATION

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
	//MoveToDestination();
	//BestPath = CustomTemp;
	//return;

	Fsm->SetNextState("Await");
	UGameManager::GetInstance()->ReleaseFromBarrier(this);
}

APathActor* AArmySoldierEnemy::GetDestination()
{
	APathActor* Dest = Cast<APathActor>(UGameManager::GetInstance()->GetPlayerNode());

	TArray<APathActor*> CustomTemp;

	if (Dest != nullptr)
	{
		if (Dest == GetCurrentNode())
		{
			TArray<APathActor*> Path;
			Path.Add(GetCurrentNode());

			BestPath = Path;
			return Path[0];
		}

		for (int i = 0; i < GetCurrentNode()->ConnectorInfo.Num(); ++i)
		{
			if (GetNodeAtCardinalDirection(GetCurrentNode()->ConnectorInfo[i].Direction) == Dest)
			{
				TArray<APathActor*> Path;
				Path.Add(GetCurrentNode());
				Path.Add(GetCurrentNode()->ConnectorInfo[i].DestinationNode);

				BestPath = Path;
				return Path[1];
			}
		}

		TArray<TArray<APathActor*>> AllLists;
		TArray<APathActor*> Blacklisted;
		TArray<APathActor*> CurList;
		Blacklisted.Add(GetCurrentNode());
		for (int i = 0; i < GetCurrentNode()->ConnectorInfo.Num(); ++i)
		{
			CurList = AStarAlgorithm(GetNodeAtCardinalDirection(GetCurrentNode()->ConnectorInfo[i].Direction), Dest, Blacklisted);
			CurList.Insert(GetCurrentNode(), 0);

			if (CurList[CurList.Num() - 1] == Dest)
			{
				AllLists.Add(CurList);
			}

		}


		if (AllLists.Num() > 0)
		{
			CustomTemp = AllLists[0];
		}
		if (AllLists.Num() > 1)
		{
			for (int i = 0; i < AllLists.Num(); ++i)
			{
				if (CustomTemp.Num() > AllLists[i].Num())
				{
					CustomTemp = AllLists[i];
				}
				else if (CustomTemp.Num() == AllLists[i].Num())
				{
					CustomTemp = CustomTemp[1] == GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS, true) ? AllLists[i] : CustomTemp;
				}
			}
		}
	}

	BestPath = CustomTemp;
	return CustomTemp[1];
}

bool AArmySoldierEnemy::IsDeadEnd(APathActor* Node)
{
	for (int i = 0; i < Node->ConnectorInfo.Num(); ++i)
	{
		if (!Node->ConnectorInfo[i].DestinationNode->Visited) { return false; }
	}
	return true;
}

TArray<APathActor*> AArmySoldierEnemy::AStarAlgorithm(APathActor* Start, APathActor* End, TArray<APathActor*> BlacklistedNodes)
{
	APathActor* Current = nullptr;

	TArray<APathActor*> OpenList;
	TArray<APathActor*> ClosedList;
	TArray<APathActor*> ListToSend;

	for (int i = 0; i < BlacklistedNodes.Num(); ++i)
	{
		ClosedList.Add(BlacklistedNodes[i]);
	}


	OpenList.Add(Start);

	while (!OpenList.IsEmpty())
	{
		APathActor* CurIT = OpenList[0];
		Current = CurIT;

		for (int i = 0; i < OpenList.Num(); ++i)
		{
			APathActor* SelectedNode = OpenList[i];

			if (SelectedNode->FScore <= Current->FScore)
			{
				if (!BlacklistedNodes.Contains(Current))
				{
					Current = SelectedNode;
					CurIT = OpenList[i];
				}
			}
		}

		if (Current == End)
		{
			break;
		}

		ClosedList.Add(Current);
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

			OpenList.Add(Current->ConnectorInfo[i].DestinationNode);

		}
	}

	//Make path list
	while (Current != nullptr)
	{
		ListToSend.Add(Current);
		Current = Current->PathfindingParent;
	}

	for (int i = 0; i < OpenList.Num(); ++i)
	{
		OpenList[i]->FScore = 1;
		OpenList[i]->PathfindingParent = nullptr;
	}

	for (int i = 0; i < ClosedList.Num(); ++i)
	{
		ClosedList[i]->FScore = 1;
		ClosedList[i]->PathfindingParent = nullptr;
	}

	Algo::Reverse(ListToSend);

	UGameManager::GetInstance()->ResetAllPathWeights();
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