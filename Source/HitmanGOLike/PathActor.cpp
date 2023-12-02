// Fill out your copyright notice in the Description page of Project Settings.


#include "PathActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CasaPlayer.h"
#include "Engine/World.h"

// Sets default values
APathActor::APathActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridPlane"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Plane.Plane")); //static permet d'executer la fonction qu'une fois
	PlaneMesh->SetStaticMesh(meshFinder.Object);

	RootComponent = PlaneMesh;
}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();

	if (StartingNode)
	{
		APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		Pawn->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, PlayerLocation.Z));

		PlayerPawn = Cast<ACasaPlayer>(Pawn);
	}
}

// Called every frame
void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APathActor::IsPlayerOnNeighbouringNode()
{
	for (int i = 0; i < NeighbouringNodes.Num(); ++i)
	{
		if (NeighbouringNodes[i]->PlayerPawn != nullptr)
		{
			TransferPlayerOwnership(*NeighbouringNodes[i]);
			return true;
		}
	}
	return false;
}

bool APathActor::IsConnectedNode(APathActor* A, APathActor* B)
{
	for (int i = 0; i < A->NeighbouringNodes.Num(); ++i)
	{
		if (A->NeighbouringNodes[i] == B)
		{
			return true;
		}
	}
	return false;
}

#if WITH_EDITOR
void APathActor::PostEditMove(bool bFinished)
{
	if (bFinished)
	{
		TArray<APathActor*> SurroundingActors;

		UE_LOG(LogTemp, Error, TEXT("Move finished"));

		for (int i = 0; i < 4; ++i)
		{
			SurroundingActors.Add(CheckNeighbourNode(i));
		}

		UMaterialInstanceDynamic* DynMat = nullptr;

		if (SurroundingActors[0])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[0]->PlaneMesh->GetMaterial(0));
			if (Up || SurroundingActors[0]->Down)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[0]->Down, "Down", 1.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Up, "Up", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[0]->PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, SurroundingActors[0]->Down, "Down", 0.0f);
			}
		}
		else {
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

			SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);
		}

		if (SurroundingActors[1])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));
			if (Right || SurroundingActors[1]->Left)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 1.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Right, "Right", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 0.0f);
			}
		}
		else {
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

			SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);
		}

		if (SurroundingActors[2])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));
			if (Down || SurroundingActors[2]->Up)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Up, "Up", 1.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Down, "Down", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Down, "Down", 0.0f);
			}
		}
		else {
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

			SetMaterialBoolParameterValue(DynMat, Down, "Down", 0.0f);
		}

		if (SurroundingActors[3])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));
			if (Left || SurroundingActors[3]->Right)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 1.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Left, "Left", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));

				SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 0.0f);
			}
		}
		else {
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));

			SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);
		}
	}	
}
#endif

void APathActor::SetMaterialBoolParameterValue(UMaterialInstanceDynamic* DynMat, bool& boolVal, FString boolValName, float value)
{
	boolVal = value == 0 ? false : true;

	if (DynMat)
	{
		DynMat->SetScalarParameterValue((TEXT("%s"), *boolValName), value);
	}
}

void APathActor::TransferPlayerOwnership(APathActor& OriginTile)
{
	PlayerPawn = OriginTile.PlayerPawn;
	OriginTile.PlayerPawn = nullptr;
}

APathActor* APathActor::CheckNeighbourNode(int direction)
{
	FHitResult HitResult;
	FlushPersistentDebugLines(GetWorld());

	FQuat RotationQuat = FQuat(FRotator(0.0f, 90.0f * direction, 0.0f));

	FVector Dest = RotationQuat.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	FVector NextNodePos = Dest * 55;
	NextNodePos.Z -= 1;

	GetWorld()->LineTraceSingleByChannel(HitResult, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10), GetActorLocation() + NextNodePos,
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50), GetActorLocation() + NextNodePos, FColor::Blue, true, 50.f);

	if (HitResult.bBlockingHit)
	{
		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			return Path;
		}
	}

	return nullptr;
}