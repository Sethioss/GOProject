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
	InitFsm();
	UGameManager::GetInstance()->RegisterToBarrier(this);
}

void AEnemyActor::RegisterToManager()
{
	if (RegisteredToManager == false)
	{
		UGameManager::GetInstance()->Enemies.Add(this);
		RegisteredToManager = true;
		Fsm->GoToNextState();
	}

	UGameManager::GetInstance()->ReleaseFromBarrier(this);
}

void AEnemyActor::InitFsm() {

	Fsm = new CasaFSM();
	CasaState* InitEnemyState = new CasaState();
	InitEnemyState->Name = "InitEnemy";

	InitEnemyState->SetUpdateDelegate(this, &AEnemyActor::RegisterToManager);

	Fsm->States.Add(InitEnemyState);
	Fsm->ChangeState("InitEnemy", false);

	CasaState* EnemyNeutralState = new CasaState();
	EnemyNeutralState->Name = "Neutral";
	EnemyNeutralState->SetUpdateDelegate(this, &AEnemyActor::NeutralTurn);
	EnemyNeutralState->SetEndDelegate(this, &AEnemyActor::EndMove);

	Fsm->States.Add(EnemyNeutralState);

	Fsm->SetNextState("Neutral");

	CasaState* AwaitState = new CasaState();
	AwaitState->Name = "Await";
	AwaitState->SetUpdateDelegate(this, &AEnemyActor::WaitUntilNextTurn);

	Fsm->States.Add(AwaitState);
}

void AEnemyActor::WaitUntilNextTurn()
{
	Fsm->SetNextState("Neutral");

	if (AllowedToMove) { Fsm->GoToNextState(); }
}

void AEnemyActor::EndMove()
{
	AllowedToMove = false;
}

void AEnemyActor::NeutralTurn() {}

APathActor* AEnemyActor::GetDestination() { return GetNodeAtCardinalDirection(EGeneralDirectionEnum::FORWARDS); }
void AEnemyActor::MoveToDestination() {}

void AEnemyActor::Attack() {}

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

	FVector NextNodePos = NormalizedDirection * ActorBounds.GetSize().X;
	FVector vec = GetActorLocation() + NextNodePos;
	vec.Z = CurrentNode->GetActorLocation().Z - 5;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), vec,
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), vec, FColor::Blue, true, 50.f);

	if (HitResult.bBlockingHit)
	{
		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			if (GetConnected)
			{
				for (int i = 0; i < Path->ConnectorInfo.Num(); ++i)
				{
					if (GetCurrentNode() == Path->ConnectorInfo[i].DestinationNode)
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

FVector AEnemyActor::GetNormalizedVectorFromDirection(EGeneralDirectionEnum Dir)
{
	FVector Vector = FVector(0.0f, 90.0f, 0.0f);
	FQuat RotationQuat = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	bool Local = false;

	if (static_cast<int>(Dir) >= 4)
	{
		Local = true;
	}

	switch (static_cast<int>(Dir) % (static_cast<int>(EGeneralDirectionEnum::VALNUM)/2))
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

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -GetActorUpVector() * 1000, ECollisionChannel::ECC_GameTraceChannel1);

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

