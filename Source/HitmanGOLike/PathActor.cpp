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

	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		ConnectorInfo[i].OriginNode = this;
	}

	if (StartingNode)
	{
		APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		Pawn->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, PlayerLocation.Z));

		PlayerPawn = Cast<ACasaPlayer>(Pawn);
	}
}

void APathActor::OnConstruction(const FTransform& Transform)
{
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		ConnectorInfo[i].OriginNode = this;
	}
}

void APathActor::SetIsEnemyPath(float value)
{
	UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	AddMaterialFloatParameterValue(DynMat, PartOfEnemyPath, "IsPartOfEnemyPath", value);
}

// Called every frame
void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APathActor::IsPlayerOnNeighbouringNodeWithoutOwnershipTransfer()
{
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		if (ConnectorInfo[i].DestinationNode->PlayerPawn != nullptr)
		{
			return true;
		}
	}
	return false;
}

APathActor* APathActor::IsPlayerOnNeighbouringNode()
{
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		if (ConnectorInfo[i].DestinationNode->PlayerPawn != nullptr)
		{
			return ConnectorInfo[i].OriginNode;
		}
	}
	return nullptr;
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

//Indique si la Foreuse est sur un Node voisin du Node actuel
APathActor* APathActor::IsForeuseOnNeighbourinNode()
{
	for (int i = 0; i < ConnectorInfo.Num(); ++i)
	{
		if (ConnectorInfo[i].DestinationNode->IsPlayerOnNeighbouringNode())
		{
			FVector TargetPos = GetActorLocation();
			FVector ForeusePos = ConnectorInfo[i].DestinationNode->GetActorLocation();
			if ((TargetPos.X == ForeusePos.X && ForeusePos.X == ConnectorInfo[i].DestinationNode->PlayerPawn->GetActorLocation().X) || (TargetPos.Y == ForeusePos.Y && ForeusePos.Y == ConnectorInfo[i].DestinationNode->PlayerPawn->GetActorLocation().Y))
			{
				return ConnectorInfo[i].DestinationNode;
			}
		}
	}
	/*for (FConnectorInfo* Node: ConnectorInfo) 
	{
		if (Node->IsPlayerOnNeighbouringNode()) 
		{
			FVector TargetPos = GetActorLocation();
			FVector ForeusePos = Node->GetActorLocation();
			if((TargetPos.X == ForeusePos.X && ForeusePos.X == Node->PlayerPawn->GetActorLocation().X)|| (TargetPos.Y == ForeusePos.Y && ForeusePos.Y == Node->PlayerPawn->GetActorLocation().Y))
			{ 
				return Node;
			}
		}
	}
	*/
	return nullptr;
}

#if WITH_EDITOR
void APathActor::PostEditMove(bool bFinished)
{
	//if (bFinished)
	//{
	//	TArray<APathActor*> SurroundingActors;

	//	for (int i = 0; i < 4; ++i)
	//	{
	//		SurroundingActors.Add(CheckNeighbourNode(static_cast<EGeneralDirectionEnum>(i)));
	//	}

	//	UMaterialInstanceDynamic* DynMat = nullptr;

	//	if (SurroundingActors[0])
	//	{
	//		DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[0]->PlaneMesh->GetMaterial(0));
	//		if (Up || SurroundingActors[0]->Down)
	//		{
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[0]->Down, "Down", 1.0f);
	//			//AddConnector(SurroundingActors[0], this, EGeneralDirectionEnum::DOWN);

	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Up, "Up", 1.0f);
	//		}
	//		else
	//		{
	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);
	//			//RemoveConnector(this, SurroundingActors[0], EGeneralDirectionEnum::UP);

	//			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[0]->PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[0]->Down, "Down", 0.0f);
	//		}
	//	}
	//	else {
	//		for (int i = 0; i < ConnectorInfo.Num(); ++i)
	//		{
	//			if (ConnectorInfo[i].Direction == EGeneralDirectionEnum::DOWN)
	//			{
	//				DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
	//				SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Down, "Down", 0.0f);
	//			}
	//		}

	//		DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//		SetMaterialBoolParameterValue(DynMat, Up, "Up", 0.0f);
	//		//RemoveConnector(this, SurroundingActors[0], EGeneralDirectionEnum::UP);
	//	}

	//	if (SurroundingActors[1])
	//	{
	//		DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));
	//		if (Right || SurroundingActors[1]->Left)
	//		{
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 1.0f);
	//			//AddConnector(SurroundingActors[1], this, EGeneralDirectionEnum::LEFT);

	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Right, "Right", 1.0f);
	//		}
	//		else
	//		{
	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);
	//			//RemoveConnector(this, SurroundingActors[1], EGeneralDirectionEnum::RIGHT);

	//			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[1]->PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[1]->Left, "Left", 0.0f);
	//		}
	//	}
	//	else {
	//		for (int i = 0; i < ConnectorInfo.Num(); ++i)
	//		{
	//			if (ConnectorInfo[i].Direction == EGeneralDirectionEnum::LEFT)
	//			{
	//				DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
	//				SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Left, "Left", 0.0f);
	//			}
	//		}
	//		DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//		SetMaterialBoolParameterValue(DynMat, Right, "Right", 0.0f);
	//		//RemoveConnector(this, SurroundingActors[1], EGeneralDirectionEnum::RIGHT);
	//	}

	//	if (SurroundingActors[2])
	//	{
	//		DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));
	//		if (Down || SurroundingActors[0]->Up)
	//		{
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Up, "Up", 1.0f);
	//			//AddConnector(SurroundingActors[2], this, EGeneralDirectionEnum::UP);

	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Down, "Down", 1.0f);
	//		}
	//		else
	//		{
	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Down, "Down", 0.0f);
	//			//RemoveConnector(this, SurroundingActors[2], EGeneralDirectionEnum::DOWN);

	//			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[2]->PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[2]->Up, "Up", 0.0f);
	//		}
	//	}
	//	else {
	//		for (int i = 0; i < ConnectorInfo.Num(); ++i)
	//		{
	//			if (ConnectorInfo[i].Direction == EGeneralDirectionEnum::UP)
	//			{
	//				DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
	//				SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Up, "Up", 0.0f);
	//			}
	//		}

	//		DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//		SetMaterialBoolParameterValue(DynMat, Down, "Down", 0.0f);
	//		//RemoveConnector(this, SurroundingActors[2], EGeneralDirectionEnum::DOWN);
	//	}

	//	if (SurroundingActors[3])
	//	{
	//		DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));
	//		if (Left || SurroundingActors[3]->Right)
	//		{
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 1.0f);
	//			//AddConnector(SurroundingActors[3], this, EGeneralDirectionEnum::RIGHT);

	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Left, "Left", 1.0f);
	//		}
	//		else
	//		{
	//			DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);
	//			//RemoveConnector(this, SurroundingActors[3], EGeneralDirectionEnum::LEFT);

	//			DynMat = Cast<UMaterialInstanceDynamic>(SurroundingActors[3]->PlaneMesh->GetMaterial(0));
	//			SetMaterialBoolParameterValue(DynMat, SurroundingActors[3]->Right, "Right", 0.0f);
	//		}
	//	}
	//	else {
	//		for (int i = 0; i < ConnectorInfo.Num(); ++i)
	//		{
	//			if (ConnectorInfo[i].Direction == EGeneralDirectionEnum::RIGHT)
	//			{
	//				DynMat = Cast<UMaterialInstanceDynamic>(ConnectorInfo[i].DestinationNode->PlaneMesh->GetMaterial(0));
	//				SetMaterialBoolParameterValue(DynMat, ConnectorInfo[i].DestinationNode->Right, "Right", 0.0f);
	//			}
	//		}
	//		DynMat = Cast<UMaterialInstanceDynamic>(PlaneMesh->GetMaterial(0));
	//		SetMaterialBoolParameterValue(DynMat, Left, "Left", 0.0f);
	//		//RemoveConnector(this, SurroundingActors[3], EGeneralDirectionEnum::LEFT);
	//	}
	//}
}
#endif

bool APathActor::CheckConnectorInfo(APathActor* CurNode, EGeneralDirectionEnum Direction)
{
	if (CurNode != nullptr)
	{
		for (int i = 0; i < CurNode->ConnectorInfo.Num(); ++i)
		{
			if (CurNode->ConnectorInfo[i].Direction == Direction)
			{
				return true;
			}
		}
	}

	return false;
}

void APathActor::AddConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction)
{
	if (!CheckConnectorInfo(CurNode, Direction))
	{
		FConnectorInfo NewInfo;

		NewInfo.DestinationNode = DestNode;
		NewInfo.Direction = Direction;
		CurNode->ConnectorInfo.Add(NewInfo);
	}

	if (Direction == EGeneralDirectionEnum::UP) { Direction = EGeneralDirectionEnum::DOWN; }
	else if (Direction == EGeneralDirectionEnum::RIGHT) { Direction = EGeneralDirectionEnum::LEFT; }
	else if (Direction == EGeneralDirectionEnum::DOWN) { Direction = EGeneralDirectionEnum::UP; }
	else if (Direction == EGeneralDirectionEnum::LEFT) { Direction = EGeneralDirectionEnum::RIGHT; }

	if (!CheckConnectorInfo(DestNode, Direction))
	{
		FConnectorInfo NewInfo;

		NewInfo.DestinationNode = CurNode;
		NewInfo.Direction = Direction;
		DestNode->ConnectorInfo.Add(NewInfo);
	}
}

void APathActor::RemoveConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction)
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

	if (Direction == EGeneralDirectionEnum::UP) { Direction = EGeneralDirectionEnum::DOWN; }
	else if (Direction == EGeneralDirectionEnum::RIGHT) { Direction = EGeneralDirectionEnum::LEFT; }
	else if (Direction == EGeneralDirectionEnum::DOWN) { Direction = EGeneralDirectionEnum::UP; }
	else if (Direction == EGeneralDirectionEnum::LEFT) { Direction = EGeneralDirectionEnum::RIGHT; }

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

void APathActor::AddMaterialFloatParameterValue(UMaterialInstanceDynamic* DynMat, float& floatVal, FString floatValName, float value)
{
	floatVal += value;

	if (DynMat)
	{
		DynMat->SetScalarParameterValue((TEXT("%s"), *floatValName), value);
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
	PlayerPawn->CurrentNode = this;
}

APathActor* APathActor::CheckNeighbourNode(EGeneralDirectionEnum Direction, bool GetConnected)
{
	FHitResult HitResult;
	FlushPersistentDebugLines(GetWorld());

	FQuat RotationQuat = FQuat(FRotator(0.0f, 90.0f * static_cast<int>(Direction), 0.0f));

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
			if (GetConnected)
			{
				for (int i = 0; i < Path->ConnectorInfo.Num(); ++i)
				{
					if (Path == ConnectorInfo[i].DestinationNode)
					{
						return Path;
					}
					return nullptr;
				}
			}
			else { return Path; }
		}



	}

	return nullptr;
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

double APathActor::ManhattanDistance(FVector StartPos, FVector EndPos)
{
	return{ FMath::Abs(StartPos.X - EndPos.X) + FMath::Abs(StartPos.Y - EndPos.Y) };
}
