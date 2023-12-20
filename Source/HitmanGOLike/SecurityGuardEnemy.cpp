// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityGuardEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameManager.h"
#include "CasaFSM.h"
#include "Kismet/KismetMathLibrary.h"

ASecurityGuardEnemy::ASecurityGuardEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASecurityGuardEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASecurityGuardEnemy::OnTurn()
{
	if (Destination)
	{
		MoveToDestination();
	}

	UGameManager::GetInstance()->RegisterToBarrier(this);
	Fsm->ChangeState("OnPostTurn");
}

void ASecurityGuardEnemy::OnPostTurn()
{
	if (UGameManager::GetInstance()->GetPlayerNode() == GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS, true))
	{
		Destination = GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS, true);
		UGameManager::GetInstance()->RegisterToBarrier(this);
		Fsm->ChangeState("OnPreAttack");
		return;
	}

	//At end
	Super::OnPostTurn();
}

void ASecurityGuardEnemy::OnPreTurn()
{
	if (IsLookingForHostage && Hostage)
	{
		Destination = GetDestinationByPathfinding(Hostage->GetCurrentNode());
	}
	else
	{
		Destination = GetDestination();
	}

	Super::OnPreTurn();
}

APathActor* ASecurityGuardEnemy::GetDestination()
{
	for (int i = 0; i < static_cast<int>(EGeneralDirectionEnum::VALNUM) / 2; i++)
	{
		APathActor* Path = GetNodeAtCardinalDirection(static_cast<EGeneralDirectionEnum>(i + ((static_cast<int>(EGeneralDirectionEnum::VALNUM) / 2))), true);
		if (Path != nullptr)
		{
			return Path;
		}
	}
	return nullptr;
}

void ASecurityGuardEnemy::MoveToDestination()
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

void ASecurityGuardEnemy::InitFsm()
{
	Super::InitFsm();
}