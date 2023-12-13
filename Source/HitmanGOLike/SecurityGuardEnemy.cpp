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

void ASecurityGuardEnemy::NeutralTurn()
{
	Destination = GetDestination();
	if (Destination != nullptr)
	{
		MoveToDestination();
	}

	UE_LOG(LogTemp, Warning, TEXT("I'm the Security Guard enemy and I'm doing my turn!"));
	UGameManager::GetInstance()->ReleaseFromBarrier(this);
}

APathActor* ASecurityGuardEnemy::GetDestination()
{
	FHitResult HitResult;

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	for (int i = 0; i < static_cast<int>(EGeneralDirectionEnum::VALNUM) / 2; i++)
	{
		APathActor* Path = GetNodeAtCardinalDirection(static_cast<EGeneralDirectionEnum>(i + (static_cast<int>(EGeneralDirectionEnum::VALNUM) / 2)));
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