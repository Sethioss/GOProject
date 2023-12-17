// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "GameManager.h"
#include "FSMBarrier.h"

struct TraversedPathElement
{
	int InDirection;
	int Visited;
	int OutDirection;
	APathActor* NodeActor;
};

struct PathToFollow
{
	int Corners;
	int Length;
	bool LeadsToTarget;
	TArray<TraversedPathElement> Elements;
};


// Sets default values
AEnemyActor::AEnemyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	Mesh->SetStaticMesh(meshFinder.Object);

	RootComponent = Mesh;
}

void AEnemyActor::Alert(AOtage* Otage)
{
	IsLookingForHostage = true;
	Stunned = true;
	Hostage = Otage;
	GetDestinationByPathfinding(Otage->GetCurrentNode());
	Fsm->ChangeState("OnPostTurn");
}

void AEnemyActor::Update()
{
	Fsm->CurrentState->FTickDelegate.ExecuteIfBound();
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentNode = SnapToGrid();

}

void AEnemyActor::Init()
{
	AllowedToMove = false;
	InitFsm();
	UGameManager::GetInstance()->RegisterToBarrier(this);
}

void AEnemyActor::ClearBestPath()
{
	if (BestPath.Num() > 0)
	{
		for (int i = 0; i < BestPath.Num(); ++i)
		{
			BestPath[i]->SetIsEnemyPath(-1);
		}
	}
}

void AEnemyActor::InitFsm() {

	Fsm = new CasaFSM();
	CasaState* InitEnemyState = new CasaState();
	InitEnemyState->Name = "OnEnemyInit";

	InitEnemyState->SetUpdateDelegate(this, &AEnemyActor::OnRegisterToManager);

	Fsm->States.Add(InitEnemyState);
	Fsm->ChangeState("OnEnemyInit", false);

	CasaState* StartGame = new CasaState();
	StartGame->Name = "OnStartGame";

	StartGame->SetUpdateDelegate(this, &AEnemyActor::OnStartGame);

	Fsm->States.Add(StartGame);

	CasaState* OnPreTurnState = new CasaState();
	OnPreTurnState->Name = "OnPreTurn";
	OnPreTurnState->SetUpdateDelegate(this, &AEnemyActor::OnPreTurn);

	Fsm->States.Add(OnPreTurnState);

	CasaState* OnTurnState = new CasaState();
	OnTurnState->Name = "OnTurn";
	OnTurnState->SetUpdateDelegate(this, &AEnemyActor::OnTurn);

	Fsm->States.Add(OnTurnState);

	CasaState* OnPostTurnState = new CasaState();
	OnPostTurnState->Name = "OnPostTurn";
	OnPostTurnState->SetUpdateDelegate(this, &AEnemyActor::OnPostTurn);

	Fsm->States.Add(OnPostTurnState);

	CasaState* OnAwaitState = new CasaState();
	OnAwaitState->Name = "OnAwait";
	OnAwaitState->SetStartDelegate(this, &AEnemyActor::RemoveRightToMove);
	OnAwaitState->SetUpdateDelegate(this, &AEnemyActor::OnAwait);

	Fsm->States.Add(OnAwaitState);

	CasaState* OnPreAttackState = new CasaState();
	OnPreAttackState->Name = "OnPreAttack";
	OnPreAttackState->SetUpdateDelegate(this, &AEnemyActor::OnPreAttack);

	Fsm->States.Add(OnPreAttackState);

	CasaState* OnAttackState = new CasaState();
	OnAttackState->Name = "OnAttack";
	OnAttackState->SetUpdateDelegate(this, &AEnemyActor::OnAttack);

	Fsm->States.Add(OnAttackState);

	CasaState* OnPostAttackState = new CasaState();
	OnPostAttackState->Name = "OnPostAttack";
	OnPostAttackState->SetUpdateDelegate(this, &AEnemyActor::OnPostAttack);

	Fsm->States.Add(OnPostAttackState);
}

void AEnemyActor::RemoveRightToMove()
{
	AllowedToMove = false;
}

APathActor* AEnemyActor::GetDestination() { return GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS, true); }
void AEnemyActor::MoveToDestination() {}

APathActor* AEnemyActor::GetDestinationByPathfinding(APathActor* DestinationPath)
{
	if (BestPath.Num() > 0)
	{
		for (int i = 0; i < BestPath.Num(); ++i)
		{
			BestPath[i]->SetIsEnemyPath(-1);
		}
	}


	APathActor* Dest = Cast<APathActor>(DestinationPath);

	TArray<APathActor*> CustomTemp;

	if (Dest != nullptr)
	{
		if (Dest == GetCurrentNode())
		{
			TArray<APathActor*> Path;
			Path.Add(GetCurrentNode());

			BestPath = Path;
			for (int i = 0; i < BestPath.Num(); ++i)
			{
				BestPath[i]->SetIsEnemyPath(1);
			}
			return Path[0];
		}

		for (int i = 0; i < GetCurrentNode()->ConnectorInfo.Num(); ++i)
		{
			if (GetCurrentNode()->ConnectorInfo[i].DestinationNode == Dest)
			{
				TArray<APathActor*> Path;
				Path.Add(GetCurrentNode());
				Path.Add(GetCurrentNode()->ConnectorInfo[i].DestinationNode);

				BestPath = Path;
				for (int j = 0; j < BestPath.Num(); ++j)
				{
					BestPath[j]->SetIsEnemyPath(1);
				}
				return Path[1];
			}
		}

		TArray<TArray<APathActor*>> AllLists;
		TArray<APathActor*> Blacklisted;
		TArray<APathActor*> CurList;
		Blacklisted.Add(GetCurrentNode());
		for (int i = 0; i < GetCurrentNode()->ConnectorInfo.Num(); ++i)
		{
			if (GetCurrentNode()->ConnectorInfo[i].DestinationNode->IsObstacle == false)
			{
				CurList = AStarAlgorithm(GetCurrentNode()->ConnectorInfo[i].DestinationNode, Dest, Blacklisted);

				if (CurList[CurList.Num() - 1] == Dest)
				{
					AllLists.Add(CurList);
				}
			}
		}

		for (int i = 0; i < AllLists.Num(); ++i)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Current path List %i"), i);
			for (int j = 0; j < AllLists[i].Num(); ++j)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *AllLists[i][j]->GetActorNameOrLabel());
			}

		}

		if (AllLists.Num() == 1)
		{
			CustomTemp = AllLists[0];
		}
		else if (AllLists.Num() > 1)
		{
			CustomTemp = AllLists[0];
			for (int i = 0; i < AllLists.Num(); ++i)
			{
				if (CustomTemp.Num() > AllLists[i].Num())
				{
					CustomTemp = AllLists[i];
				}
				else if (CustomTemp.Num() == AllLists[i].Num())
				{
					if (GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS, true) == AllLists[i][0])
					{
						CustomTemp = AllLists[i];
					}
					else if (GetNodeAtCardinalDirection(EGeneralDirectionEnum::LOCALRIGHT, true) == AllLists[i][0])
					{
						CustomTemp = AllLists[i];
					}
				}
			}

			for (int j = 0; j < CustomTemp.Num(); ++j)
			{
				UE_LOG(LogTemp, Warning, TEXT("Custom temp: %s"), *CustomTemp[j]->GetActorNameOrLabel());
			}
		}
		else
		{
			return nullptr;
		}
	}

	BestPath = CustomTemp;

	if (BestPath.Num() > 0)
	{
		for (int i = 0; i < BestPath.Num(); ++i)
		{
			BestPath[i]->SetIsEnemyPath(1);
		}
	}


	return CustomTemp[0];
}

TArray<APathActor*> AEnemyActor::AStarAlgorithm(APathActor* Start, APathActor* End, TArray<APathActor*> BlacklistedNodes)
{
	APathActor* Current = nullptr;

	TArray<APathActor*> OpenList;
	TArray<APathActor*> ClosedList;
	TArray<APathActor*> ListToSend;

	for (int i = 0; i < BlacklistedNodes.Num(); ++i)
	{
		ClosedList.Add(BlacklistedNodes[i]);
	}


	OpenList.Add(Start);

	while (!OpenList.IsEmpty())
	{
		APathActor* CurIT = OpenList[0];
		Current = CurIT;

		for (int i = 0; i < OpenList.Num(); ++i)
		{
			APathActor* SelectedNode = OpenList[i];

				if (SelectedNode->FScore <= Current->FScore)
				{
					if (!BlacklistedNodes.Contains(Current))
					{
						Current = SelectedNode;
						CurIT = OpenList[i];
					}
				}
		}

		if (Current == End)
		{
			break;
		}

		ClosedList.Add(Current);
		OpenList.Remove(CurIT);

		for (int i = 0; i < Current->ConnectorInfo.Num(); ++i)
		{
			//If node has already been visited, don't add to openlist
			if (ClosedList.Contains(Current->ConnectorInfo[i].DestinationNode))
			{
				continue;
			}

			if (Current->IsObstacle == false)
			{
				APathActor* Successor = Current->ConnectorInfo[i].DestinationNode;
				Current->ConnectorInfo[i].DestinationNode->FScore = ManhattanDistance(Current->ConnectorInfo[i].DestinationNode->GetActorLocation(), End->GetActorLocation());
				if (Current->IsObstacle) { Current->ConnectorInfo[i].DestinationNode->FScore = 99999; }
				if (Current->PathSecondary) { Current->ConnectorInfo[i].DestinationNode->FScore = 99999; }
				Successor->PathfindingParent = Current;

				OpenList.Add(Current->ConnectorInfo[i].DestinationNode);
			}
		}
	}

	//Make path list
	while (Current != nullptr)
	{
		if (Current->IsWalkableNode)
		{
			ListToSend.Add(Current);
		}
		Current = Current->PathfindingParent;
	}

	for (int i = 0; i < OpenList.Num(); ++i)
	{
		OpenList[i]->FScore = 1;
		OpenList[i]->PathfindingParent = nullptr;
	}

	for (int i = 0; i < ClosedList.Num(); ++i)
	{
		ClosedList[i]->FScore = 1;
		ClosedList[i]->PathfindingParent = nullptr;
	}

	Algo::Reverse(ListToSend);

	UGameManager::GetInstance()->ResetAllPathWeights();
	return ListToSend;
}

float AEnemyActor::ManhattanDistance(FVector A, FVector B)
{
	float X = A.X - B.X;
	float Y = A.Y - B.Y;

	float h = FMath::Abs(X) + FMath::Abs(Y);
	return h;
}

APathActor* AEnemyActor::GetCurrentNode()
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -GetActorUpVector() * 1000, ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - (GetActorUpVector() * 1000), FColor::Green, true, 50.f);

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

APathActor* AEnemyActor::GetNodeAtCardinalDirection(EGeneralDirectionEnum Dir, bool GetConnected)
{
	FHitResult HitResult;

	FVector NormalizedDirection = GetNormalizedVectorFromDirection(Dir);

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	FVector NextNodePos = NormalizedDirection * ActorBounds.GetSize().X * 2;
	FVector vec = GetActorLocation() + NextNodePos;
	vec.Z = CurrentNode->GetActorLocation().Z - 5;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), vec,
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), vec, FColor::Blue, true, 50.f);

	if (HitResult.bBlockingHit)
	{
		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path != nullptr)
		{
			if (!Path->IsObstacle)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Path->GetActorNameOrLabel());
				if (GetConnected)
				{
					for (int i = 0; i < Path->ConnectorInfo.Num(); ++i)
					{

						if (GetCurrentNode() == Path->ConnectorInfo[i].DestinationNode)
						{
							return Path;
						}
					}
				}
				else { return Path; }
			}
		}
	}

	return nullptr;
}

FVector AEnemyActor::GetNormalizedVectorFromDirection(EGeneralDirectionEnum Dir)
{
	FVector Vector = FVector(0.0f, 90.0f, 0.0f);
	FQuat RotationQuat = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	bool Local = false;

	if (static_cast<int>(Dir) >= 4)
	{
		Local = true;
	}

	switch (static_cast<int>(Dir) % (static_cast<int>(EGeneralDirectionEnum::VALNUM) / 2))
	{
	case 0:
	{
		RotationQuat = FQuat(FRotator(0.0f, 90.0f * 0, 0.0f));
		break;
	}
	case 1:
	{
		RotationQuat = FQuat(FRotator(0.0f, 90.0f * 1, 0.0f));
		break;
	}
	case 2:
	{
		RotationQuat = FQuat(FRotator(0.0f, 90.0f * 2, 0.0f));
		break;
	}
	case 3:
	{
		RotationQuat = FQuat(FRotator(0.0f, 90.0f * 3, 0.0f));
		break;
	}
	}

	Vector = Local ? RotationQuat.RotateVector(GetActorForwardVector()) : RotationQuat.Vector();
	return Vector;
}

APathActor* AEnemyActor::SnapToGrid(FVector offset)
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -(GetActorUpVector() * 1000), ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - (GetActorUpVector() * 1000), FColor::Green, true, 50.f);

	if (HitResult.bBlockingHit)
	{

		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			CurrentNode = Path;

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z + (ActorBounds.GetSize().Z / 2)));
		}
	}

	return CurrentNode;
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyActor::OnRegisterToManager()
{
	UGameManager::GetInstance()->Enemies.Add(this);
	RegisteredToManager = true;
	AllowedToMove = false;
	UGameManager::GetInstance()->ReleaseFromBarrier(this);

	Fsm->ChangeState("OnStartGame");

}

void AEnemyActor::OnStartGame()
{
	Fsm->ChangeState("OnAwait");
}

void AEnemyActor::OnAwait()
{
	if (AllowedToMove) { Fsm->ChangeState("OnPreTurn"); }
}

void AEnemyActor::OnPreTurn() 
{
	if (!Stunned)
	{
		if (Destination)
		{
			if (Destination == UGameManager::GetInstance()->GetPlayerNode())
			{
				UGameManager::GetInstance()->RegisterToBarrier(this);
				Fsm->ChangeState("OnPreAttack");
			}
			else
			{
				Fsm->ChangeState("OnTurn");
			}
		}
		else
		{
			UGameManager::GetInstance()->ReleaseFromBarrier(this);
			Fsm->ChangeState("OnAwait");
		}
	}
}

void AEnemyActor::OnStandby() {

}

void AEnemyActor::OnTurn() {}
void AEnemyActor::OnPostTurn()
{
	if (!Stunned)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Post turn"));
		if (IsLookingForHostage && Hostage)
		{
			if (GetCurrentNode() == Hostage->GetCurrentNode())
			{
				Hostage->SetActorLocation(FVector(100000, 100000, 100000));
				UGameManager::GetInstance()->UnregisterHostage(Hostage);
				UE_LOG(LogTemp, Warning, TEXT("Hostage found! retrieving..."));

			}
		}
	}

	if (Stunned) { Stunned = false; }

	AllowedToMove = false;
	UGameManager::GetInstance()->ReleaseFromBarrier(this);
	Fsm->ChangeState("OnAwait");
}

void AEnemyActor::OnPreAttack() { UE_LOG(LogTemp, Warning, TEXT("I'm pre-attacking")); Fsm->ChangeState("OnAttack"); }
void AEnemyActor::OnAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm attacking"));
	MoveToHalfDestination();
	UGameManager::GetInstance()->KillPlayer();
	Fsm->ChangeState("OnAwait");
}
void AEnemyActor::OnPostAttack() {}

void AEnemyActor::MoveToHalfDestination() 
{
	if (Destination)
	{

		FVector2D diff = FVector2D(Destination->GetActorLocation().X - GetActorLocation().Y);

		float AngleToRotate = FMath::Atan2(Destination->GetActorLocation().Y - GetActorLocation().Y, Destination->GetActorLocation().X - GetActorLocation().X);

		// Convert the angle from radians to degrees
		float AngleInDegrees = FMath::RadiansToDegrees(AngleToRotate);

		FQuat RotationQuat = FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(AngleInDegrees));

		// Set the new rotation to your object
		SetActorRotation(RotationQuat.Rotator());

		diff = FVector2D(Destination->GetActorLocation() - GetActorLocation());
		SetActorLocation(FVector(Destination->GetActorLocation().X - (diff.X/2), Destination->GetActorLocation().Y - (diff.Y / 2), GetActorLocation().Z));
	}
}