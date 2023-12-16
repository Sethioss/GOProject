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

void AArmySoldierEnemy::OnTurn()
{
	if (IsLookingForHostage)
	{
		Destination = GetDestinationByPathfinding(Hostage->GetCurrentNode());
	
		if (Destination)
		{
			MoveToDestination();
		}
	}
	else 
	{
		Destination = GetDestinationByPathfinding(UGameManager::GetInstance()->GetPlayerNode());

		if (Destination)
		{
			MoveToDestination();
		}
	}

	UGameManager::GetInstance()->RegisterToBarrier(this);
	Fsm->ChangeState("OnPostTurn");
}

bool AArmySoldierEnemy::IsDeadEnd(APathActor* Node)
{
	for (int i = 0; i < Node->ConnectorInfo.Num(); ++i)
	{
		if (!Node->ConnectorInfo[i].DestinationNode->Visited) { return false; }
	}
	return true;
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

		SetActorLocation(FVector(Destination->GetActorLocation().X, Destination->GetActorLocation().Y, GetActorLocation().Z));
		CurrentNode = Destination;
		Destination = nullptr;
	}
}

void AArmySoldierEnemy::InitFsm()
{
	Super::InitFsm();
}