// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmySoldierEnemy.h"
#include "EnemyActor.h"
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

	TArray<TArray<APathActor*>> PathLists;

	if (Dest != nullptr)
	{
		for (int i = 0; i < CurrentNode->NeighbouringNodes.Num(); ++i)
		{
			//Autre fonction GetBESTPathFromConnector
			PathLists.Add(GetPathFromConnector(CurrentNode->NeighbouringNodes[i], Dest, CurrentPath));
		}
	}

	TArray<APathActor*> FinalPath;
	int Temp = 0;

	FinalPath.Add(CurrentNode);

	for (int i = 0; i < PathLists.Num(); ++i)
	{
		if (PathLists[i].Num() > Temp) { FinalPath = PathLists[i]; }
	}

	CurrentPath = FinalPath;
	
	//return GetDestFromPath(CurrentPath);
	return nullptr;
}

APathActor* AArmySoldierEnemy::GetDestFromPath(TArray<APathActor*> Path)
{
	return SnapToGrid();
}

TArray<APathActor*> AArmySoldierEnemy::GetPathFromConnector(APathActor* Start, APathActor* Dest, TArray<APathActor*> List)
{
	//Get all next neighbours
	TArray<APathActor*> Neighbours;

	for (int i = 0; i < CurrentNode->NeighbouringNodes.Num(); ++i)
	{
		//Si noeud actuel n'est pas dans la liste de noeuds visités
		if (!Start->NodeVisitedForPathfinding)
		{
			if (CurrentNode->NeighbouringNodes[i] != Start)
			{
				Neighbours.Add(CurrentNode->NeighbouringNodes[i]);
				Start->NodeVisitedForPathfinding = true;
			}
		}
	}

	if (Neighbours.Num() != 0 || Start == Dest)
	{
		if (Start != Dest)
		{
			// Invalidate path
			// 
			//Get All the possible paths starting from this node
			for (int i = 0; i < Neighbours.Num(); ++i)
			{
				GetPathFromConnector(Neighbours[i], Dest, List);
			}
		}
		else 
		{
			//Retourner la liste actuelle
			return List;
		}
	}
	else
	{
		List.Add(Start);
		Dest = Start;
	}

	return Neighbours;
}

void AArmySoldierEnemy::MoveToDestination()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm the army soldier and I'm moving!"));
}

void AArmySoldierEnemy::InitFsm()
{
	Super::InitFsm();
}