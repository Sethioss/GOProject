// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "CasaPlayer.h"

UGameManager* UGameManager::Instance = nullptr;

// Sets default values for this component's properties
UGameManager::UGameManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (!Instance)
	{
		Instance = this;
		// Ensure the instance is not garbage collected
		Instance->AddToRoot();

	}

	AActor* act;
	act = UGameplayStatics::GetActorOfClass(GetWorld(), ACasaPlayer::StaticClass());

	ACasaPlayer* pl = Cast<ACasaPlayer>(act);

	if (pl != nullptr) {
		Instance->ElementsToRegister += 1;
	}

	// ...
	TArray<AActor*> EnemiesToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActor::StaticClass(), EnemiesToFind);
	
	Instance->ElementsToRegister = EnemiesToFind.Num();

	InitFsm();
}

void UGameManager::InitGame() 
{
	UE_LOG(LogTemp, Warning, TEXT("I Init"));
	if (Instance->ElementsToRegister <= 0)
	{
		BlockPlayerInput();
		Instance->Fsm.ChangeState("OnAwaitingPlayerInput");
	}
}

void UGameManager::OnAwaitPlayerInput()
{
	if (Instance->Player->TurnFinished)
	{
		Instance->Fsm.ChangeState("OnEnemyTurn");
	}
}

void UGameManager::BlockPlayerInput()
{
	Instance->Player->ShouldMove = false;
}

void UGameManager::ReleasePlayerInput()
{
	Instance->Player->ShouldMove = true;
}

void UGameManager::InitFsm()
{
	CasaState* InitGame = new CasaState();
	InitGame->Name = "InitiatingGame";
	InitGame->SetUpdateDelegate(this, &UGameManager::InitGame);
	Instance->Fsm.States.Add(InitGame);
	Instance->Fsm.ChangeState("InitiatingGame", false);

	CasaState* AwaitingPlayerInput = new CasaState();
	AwaitingPlayerInput->Name = "OnAwaitingPlayerInput";
	AwaitingPlayerInput->SetStartDelegate(this, &UGameManager::ReleasePlayerInput);
	AwaitingPlayerInput->SetUpdateDelegate(this, &UGameManager::OnAwaitPlayerInput);
	AwaitingPlayerInput->SetEndDelegate(this, &UGameManager::BlockPlayerInput);
	Instance->Fsm.States.Add(AwaitingPlayerInput);


	CasaState* OnEnemyTurn = new CasaState();
	OnEnemyTurn->Name = "OnEnemyTurn";
	OnEnemyTurn->SetUpdateDelegate(this, &UGameManager::OnEnemyTurn);
	Instance->Fsm.States.Add(OnEnemyTurn);


	CasaState* DeathState = new CasaState();
	DeathState->Name = "OnDeath";
	DeathState->SetStartDelegate(this, &UGameManager::BlockPlayerInput);
	DeathState->SetUpdateDelegate(this, &UGameManager::OnPlayerDeath);
	Instance->Fsm.States.Add(DeathState);
}


// Called every frame
void UGameManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Instance->Fsm.CurrentState->FTickDelegate.Execute();
}

void UGameManager::OnEnemyTurn()
{
	for (AEnemyActor* Enemy : Instance->Enemies)
	{
		Enemy->Update();
	}

	Instance->Player->TurnFinished = false;

	Instance->Fsm.ChangeState("OnAwaitingPlayerInput");
}

UGameManager* UGameManager::GetInstance()
{
	return Instance;
}

void UGameManager::OnPlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm dead skull emoji"));
}

APathActor* UGameManager::GetPlayerNode()
{
	ACasaPlayer* Pl = Cast<ACasaPlayer>(Instance->Player);

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, Pl->GetActorLocation(), (Pl->GetActorUpVector() * -1) * 1000, ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), Pl->GetActorLocation(), Pl->GetActorLocation() - (Pl->GetActorUpVector() * 1000), FColor::Green, true, 50.f);

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
