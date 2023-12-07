// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "GameManager.h"

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
	Fsm.CurrentState->FTickDelegate.ExecuteIfBound();
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentNode = SnapToGrid();
	InitFsm();
	RegisterToManager();
}

void AEnemyActor::RegisterToManager()
{
	if (RegisteredToManager == false)
	{
		UGameManager::GetInstance()->Enemies.Add(this);
		RegisteredToManager = true;
		UGameManager::GetInstance()->ElementsToRegister -= 1;
		Fsm.ChangeState("Neutral");
	}
}

void AEnemyActor::InitFsm() {

	CasaState* InitEnemyState = new CasaState();
	InitEnemyState->Name = "InitEnemy";

	InitEnemyState->SetUpdateDelegate(this, &AEnemyActor::RegisterToManager);

	Fsm.States.Add(InitEnemyState);
	Fsm.ChangeState("InitEnemy", false);

	CasaState* EnemyNeutralState = new CasaState();
	EnemyNeutralState->Name = "Neutral";

	EnemyNeutralState->SetUpdateDelegate(this, &AEnemyActor::NeutralTurn);

	Fsm.States.Add(EnemyNeutralState);
}

void AEnemyActor::NeutralTurn() {}

APathActor* AEnemyActor::GetDestination() { return GetNodeAtCardinalDirection(EEnemyDirectionEnum::FORWARDS); }
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
			CurrentNode = Path;

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z + (ActorBounds.GetExtent().Z)));
		}
	}

	return CurrentNode;
}

APathActor* AEnemyActor::GetNodeAtCardinalDirection(EEnemyDirectionEnum Dir)
{
	FHitResult HitResult;

	FVector NormalizedDirection = GetNormalizedVectorFromDirection(Dir);

	FBox ActorBounds = CurrentNode->GetComponentsBoundingBox();

	FVector NextNodePos = NormalizedDirection * ActorBounds.GetSize().X;
	FVector vec = GetActorLocation() + NextNodePos;
	vec.Z = CurrentNode->GetActorLocation().Z - 5;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), vec,
		ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), vec, FColor::White, true, 50.f);

	if (HitResult.bBlockingHit)
	{
		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path != nullptr)
		{
			return Path;
		}
	}

	return nullptr;
}

FVector AEnemyActor::GetNormalizedVectorFromDirection(EEnemyDirectionEnum Dir)
{
	FVector Vector = FVector(0.0f, 90.0f, 0.0f);
	FQuat RotationQuat = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	bool Local = false;

	if (static_cast<int>(Dir) >= 4)
	{
		Local = true;
	}

	switch (static_cast<int>(Dir) % (static_cast<int>(EEnemyDirectionEnum::VALNUM)/2))
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

