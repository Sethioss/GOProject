// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmySoldierEnemy.h"
#include "Algo/Reverse.h"
#include "GameManager.h"
#include "PathActor.h"
#include "Containers/Array.h"

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

void AArmySoldierEnemy::OnPreTurn()
{

	if (IsLookingForHostage)
	{
		Destination = GetDestinationByPathfinding(Hostage->GetCurrentNode());
	}
	else 
	{
		for (int i = 0; i < BestPath.Num(); ++i)
		{
			if (BestPath[i]->IsObstacle)
			{
				Destination = GetDestinationByPathfinding(UGameManager::GetInstance()->GetPlayerNode());
			}
		}
		if (!Destination)
		{
			Destination = GetDestinationByPathfinding(UGameManager::GetInstance()->GetPlayerNode());
		}
	}

	Super::OnPreTurn();
}

void AArmySoldierEnemy::OnTurn()
{
	if (Destination)
	{
		if (UGameManager::GetInstance()->JustChangedSubLevel)
		{
			UGameManager::GetInstance()->JustChangedSubLevel = false;
			UGameManager::GetInstance()->ReleaseFromBarrier(this);
			Fsm->ChangeState("OnAwait");
			return;
		}
		else {
			MoveToDestination();
		}
		if (FVector::Distance(GetActorLocation(), Destination->GetActorLocation()) < 20.f)
		{
			UGameManager::GetInstance()->RegisterToBarrier(this);
			SetActorLocation(Destination->GetActorLocation());
			CurrentNode = Destination;
			Fsm->ChangeState("OnPostTurn");
		}
	}
	else {
		UGameManager::GetInstance()->RegisterToBarrier(this);
		Fsm->ChangeState("OnPostTurn");
	}
}

void AArmySoldierEnemy::OnPostTurn()
{
	if (IsLookingForHostage)
	{
		Destination = GetDestinationByPathfinding(Hostage->GetCurrentNode());
	}
	else
	{
		Destination = GetDestinationByPathfinding(UGameManager::GetInstance()->GetPlayerNode());
	}
	Super::OnPostTurn();
}

void AArmySoldierEnemy::OnPreAttack()
{
	ClearBestPath();
	Super::OnPreAttack();
}

bool AArmySoldierEnemy::IsDeadEnd(APathActor* Node)
{
	for (int i = 0; i < Node->ConnectorInfo.Num(); ++i)
	{
		if (!Node->ConnectorInfo[i].DestinationNode->Visited) { return false; }
	}
	return true;
}

void AArmySoldierEnemy::Init()
{
	Super::Init();
}

void AArmySoldierEnemy::OnStartGame()
{
	if (UGameManager::GetInstance()->GetPlayerNode())
	{
		Destination = GetDestinationByPathfinding(UGameManager::GetInstance()->GetPlayerNode());
		Super::OnStartGame();
	}
}

void AArmySoldierEnemy::MoveToDestination()
{
	if (Destination)
	{
		FVector2f diff = FVector2f(Destination->GetActorLocation().X - GetActorLocation().Y);

		float AngleToRotate = FMath::Atan2(Destination->GetActorLocation().Y - GetActorLocation().Y, Destination->GetActorLocation().X - GetActorLocation().X);

		// Convert the angle from radians to degrees
		float AngleInDegrees = FMath::RadiansToDegrees(AngleToRotate);

		FQuat RotationQuat = FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(AngleInDegrees));

		// Set the new rotation to your object
		SetActorRotation(RotationQuat.Rotator());

		if (FVector::Distance(GetActorLocation(), Destination->GetActorLocation()) > 20.f)
		{
			FVector Move = FMath::InterpEaseInOut<FVector>(GetActorLocation(), Destination->GetActorLocation(), GetWorld()->GetDeltaSeconds(), 0.3f);
			SetActorLocation(Move);
		}

	}
}

void AArmySoldierEnemy::InitFsm()
{
	Super::InitFsm();
}