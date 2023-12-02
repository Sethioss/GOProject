// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityGuardEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "CasaFSM.h"
#include "Kismet/KismetMathLibrary.h"

// Called when the game starts or when spawned
void ASecurityGuardEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASecurityGuardEnemy::UpdateBehaviour()
{
	switch (State) {
		case EEnemyState::NEUTRAL:
			PatrolStep();
			MoveToDestination();
	}

}

void ASecurityGuardEnemy::BeginTurnBehaviour()
{
	//UE_LOG(LogTemp, Warning, TEXT("I'm the security guard, and I begin my turn!"));
}

ASecurityGuardEnemy::ASecurityGuardEnemy()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASecurityGuardEnemy::MoveToDestination()
{
	if (Destination) {

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
	CasaState* StandardState = new CasaState();
	StandardState->Name = "Standard";

	StandardState->SetStartState(this, &ASecurityGuardEnemy::BeginTurnBehaviour);
	StandardState->SetUpdateState(this, &ASecurityGuardEnemy::UpdateBehaviour);
	StandardState->SetEndState(this, &ASecurityGuardEnemy::EndTurnBehaviour);

	Fsm.States.Add(StandardState);
	Fsm.ChangeState(StandardState);
}

void ASecurityGuardEnemy::PatrolStep() 
{
	FHitResult HitResult;

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	for (int i = 0; i < 4; ++i)
	{
		FQuat RotationQuat = FQuat(FRotator(0.0f, 90.0f * i, 0.0f));

		FVector direction = RotationQuat.RotateVector(GetActorForwardVector());

		FString str = ActorBounds.ToString();

		FVector NextNodePos = direction * ((static_cast<float>(direction.Y) == 0 ? ActorBounds.GetSize().X : ActorBounds.GetSize().Y));
		FVector vec = GetActorLocation() + NextNodePos;
		vec.Z = CurrentNode->GetActorLocation().Z - 5;

		GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), vec,
			ECollisionChannel::ECC_GameTraceChannel1);

		DrawDebugLine(GetWorld(), GetActorLocation(), vec, FColor::White, true, 50.f);

		if (HitResult.bBlockingHit)
		{
			APathActor* Path = Cast<APathActor>(HitResult.GetActor());

			if (Path)
			{
				if (CurrentNode->IsConnectedNode(CurrentNode, Path))
				{
					Destination = Path;
					return;
				}
			}
		}
	}
	
}

void ASecurityGuardEnemy::AlertedStart()
{
}

void ASecurityGuardEnemy::AlertedStep()
{
}

void ASecurityGuardEnemy::AlertedEnd()
{
}

void ASecurityGuardEnemy::Attack()
{
}
