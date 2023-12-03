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
	GetDestination();
	MoveToDestination();
}

void AArmySoldierEnemy::GetDestination()
{
	FHitResult HitResult;

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	APathActor* Dest = Cast<APathActor>(UGameManager::GetInstance()->GetPlayerNode());

	TArray<TArray<APathActor*>> PathLists;

	if (Dest != nullptr)
	{
		for (int i = 0; i < CurrentNode->NeighbouringNodes.Num(); ++i)
		{
			PathLists.Add(GetPathTo(Dest));
		}
	}

	TArray<APathActor*> FinalPath;
	int Temp = 0;

	for (int i = 0; i < PathLists.Num(); ++i)
	{
		if (PathLists[i].Num() > Temp) { FinalPath = PathLists[i]; }
	}

	CurrentPath = FinalPath;
	Destination = GetDestFromPath(CurrentPath);
}

APathActor* AArmySoldierEnemy::GetDestFromPath(TArray<APathActor*> Path)
{
	return Path[0];
}

TArray<APathActor*> AArmySoldierEnemy::GetPathTo(APathActor* Dest)
{
	//Algorithm to calculate path
	return CurrentPath;
}

void AArmySoldierEnemy::MoveToDestination()
{

}

void AArmySoldierEnemy::InitFsm()
{

}