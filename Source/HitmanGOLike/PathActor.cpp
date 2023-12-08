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
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		if (ConnectorInfo[i].DestinationNode->PlayerPawn != nullptr)
		{
			TransferPlayerOwnership(*ConnectorInfo[i].DestinationNode);
			return true;
		}
	}
	return false;
}

bool APathActor::IsConnectedNode(APathActor* A, APathActor* B)
{
	for (int i = 0; i < A->ConnectorInfo.Num(); ++i)
	{
		if (A->ConnectorInfo[i].DestinationNode == B)
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
				AddConnector(SurroundingActors[0], this, EPathDirectionEnum::DOWN);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Up, "Up", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);
				RemoveConnector(this, SurroundingActors[0], EPathDirectionEnum::UP);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[0]->PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[0]->Down, "Down", 0.0f);
			}
		}
		else {
			for (int i = 0; i < ConnectorInfo.Num(); ++i)
			{
				if (ConnectorInfo[i].Direction == EPathDirectionEnum::DOWN)
				{
					DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
					SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode
->Down, "Down", 0.0f);
				}
			}

			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
			SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);
			RemoveConnector(this, SurroundingActors[0], EPathDirectionEnum::UP);
		}

		if (SurroundingActors[1])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));
			if (Right || SurroundingActors[1]->Left)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 1.0f);
				AddConnector(SurroundingActors[1], this, EPathDirectionEnum::LEFT);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Right, "Right", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);
				RemoveConnector(this, SurroundingActors[1], EPathDirectionEnum::RIGHT);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 0.0f);
			}
		}
		else {
			for (int i = 0; i < ConnectorInfo.Num(); ++i)
			{
				if (ConnectorInfo[i].Direction == EPathDirectionEnum::LEFT)
				{
					DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
					SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Left, "Left", 0.0f);
				}
			}
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
			SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);
			RemoveConnector(this, SurroundingActors[1], EPathDirectionEnum::RIGHT);
		}

		if (SurroundingActors[2])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));
			if (Down || SurroundingActors[0]->Up)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Up, "Up", 1.0f);
				AddConnector(SurroundingActors[2], this, EPathDirectionEnum::UP);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Down, "Down", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Down, "Down", 0.0f);
				RemoveConnector(this, SurroundingActors[2], EPathDirectionEnum::DOWN);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Up, "Up", 0.0f);
			}
		}
		else {
			for (int i = 0; i < ConnectorInfo.Num(); ++i)
			{
				if (ConnectorInfo[i].Direction == EPathDirectionEnum::UP)
				{
					DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
					SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Up, "Up", 0.0f);
				}
			}

			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
			SetMaterialBoolParameterValue(DynMat, Down, "Down", 0.0f);
			RemoveConnector(this, SurroundingActors[2], EPathDirectionEnum::DOWN);
		}

		if (SurroundingActors[3])
		{
			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));
			if (Left || SurroundingActors[3]->Right)
			{
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 1.0f);
				AddConnector(SurroundingActors[3], this, EPathDirectionEnum::RIGHT);

				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Left, "Left", 1.0f);
			}
			else
			{
				DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);
				RemoveConnector(this, SurroundingActors[3], EPathDirectionEnum::LEFT);

				DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));
				SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 0.0f);
			}
		}
		else {
			for (int i = 0; i < ConnectorInfo.Num(); ++i)
			{
				if (ConnectorInfo[i].Direction == EPathDirectionEnum::RIGHT)
				{
					DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
					SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Right, "Right", 0.0f);
				}
			}
			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
			SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);
			RemoveConnector(this, SurroundingActors[3], EPathDirectionEnum::LEFT);
		}
	}
}
#endif

bool APathActor::CheckConnectorInfo(APathActor* CurNode, EPathDirectionEnum Direction)
{
	for (int i = 0; i < CurNode->ConnectorInfo.Num(); ++i)
	{
		if (CurNode->ConnectorInfo[i].Direction == Direction)
		{
			return true;
		}
	}
	return false;
}

void APathActor::AddConnector(APathActor* CurNode, APathActor* DestNode, EPathDirectionEnum Direction)
{
	if (!CheckConnectorInfo(CurNode, Direction))
	{
		FConnectorInfo NewInfo;

		NewInfo.DestinationNode = DestNode;
		NewInfo.Direction = Direction;
		CurNode->ConnectorInfo.Add(NewInfo);
	}

	if (Direction == EPathDirectionEnum::UP) { Direction = EPathDirectionEnum::DOWN; }
	else if (Direction == EPathDirectionEnum::RIGHT) { Direction = EPathDirectionEnum::LEFT; }
	else if (Direction == EPathDirectionEnum::DOWN) { Direction = EPathDirectionEnum::UP; }
	else if (Direction == EPathDirectionEnum::LEFT) { Direction = EPathDirectionEnum::RIGHT; }

	if (!CheckConnectorInfo(DestNode, Direction))
	{
		FConnectorInfo NewInfo;

		NewInfo.DestinationNode = CurNode;
		NewInfo.Direction = Direction;
		DestNode->ConnectorInfo.Add(NewInfo);
	}
}

void APathActor::RemoveConnector(APathActor* CurNode, APathActor* DestNode, EPathDirectionEnum Direction)
{
	if (CheckConnectorInfo(CurNode, Direction))
	{
		for (int i = 0; i < CurNode->ConnectorInfo.Num(); ++i)
		{
			if (CurNode->ConnectorInfo[i].DestinationNode == DestNode)
			{
				if (CurNode->ConnectorInfo[i].Direction == Direction)
				{
					CurNode->ConnectorInfo.Remove(CurNode->ConnectorInfo[i]);
					return;
				}
			}
		}
	}

	if (Direction == EPathDirectionEnum::UP) { Direction = EPathDirectionEnum::DOWN; }
	else if (Direction == EPathDirectionEnum::RIGHT) { Direction = EPathDirectionEnum::LEFT; }
	else if (Direction == EPathDirectionEnum::DOWN) { Direction = EPathDirectionEnum::UP; }
	else if (Direction == EPathDirectionEnum::LEFT) { Direction = EPathDirectionEnum::RIGHT; }

	if (CheckConnectorInfo(DestNode, Direction))
	{
		for (int i = 0; i < DestNode->ConnectorInfo.Num(); ++i)
		{
			if (DestNode->ConnectorInfo[i].DestinationNode == CurNode)
			{
				if (DestNode->ConnectorInfo[i].Direction == Direction)
				{
					DestNode->ConnectorInfo.Remove(DestNode->ConnectorInfo[i]);
					return;
				}
			}
		}
	}
}

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

APathActor* APathActor::CheckNeighbourNode(int Direction, bool GetConnected)
{
	FHitResult HitResult;
	FlushPersistentDebugLines(GetWorld());

	FQuat RotationQuat = FQuat(FRotator(0.0f, 90.0f * Direction, 0.0f));

	FVector Dest = RotationQuat.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	FVector NextNodePos = Dest * 55;
	NextNodePos.Z -= 1;

	GetWorld()->LineTraceSingleByChannel(HitResult, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10), GetActorLocation() + NextNodePos,
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50), GetActorLocation() + NextNodePos, FColor::Blue, true, 50.f);

	if (HitResult.bBlockingHit)
	{
		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (GetConnected)
		{
			for (int i = 0; i < Path->ConnectorInfo.Num(); ++i)
			{
				if (Path == ConnectorInfo[i].DestinationNode)
				{
					return Path;
				}
			}
		}
		else { return Path; }

	}

	return nullptr;
}