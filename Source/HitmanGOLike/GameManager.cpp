// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "CasaPlayer.h"

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

	// ...
	TArray<AActor*> EnemiesToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActor::StaticClass(), EnemiesToFind);

	for (AActor* enemy : EnemiesToFind)
	{
		AEnemyActor* MyActor = Cast<AEnemyActor>(enemy);

		if (MyActor != nullptr) {
			// Do what ever you need with your MyActor
			Enemies.Add(MyActor);
		}
	}

	AActor* act;
	act = UGameplayStatics::GetActorOfClass(GetWorld(), ACasaPlayer::StaticClass());

	ACasaPlayer* pl = Cast<ACasaPlayer>(act);

	if (pl != nullptr) {
		Player = pl;
	}

	InitFsm();
}

void UGameManager::BlockPlayerInput()
{
	Player->ShouldMove = false;
}

void UGameManager::ReleasePlayerInput()
{
	Player->ShouldMove = true;
}

void UGameManager::InitFsm()
{
	CasaState* AwaitingPlayerInput = new CasaState();
	AwaitingPlayerInput->Name = "OnAwaitingPlayerInput";
	AwaitingPlayerInput->SetStartDelegate(this, &UGameManager::ReleasePlayerInput);
	AwaitingPlayerInput->SetUpdateDelegate(this, &UGameManager::OnAwaitPlayerInput);
	AwaitingPlayerInput->SetEndDelegate(this, &UGameManager::BlockPlayerInput);
	Fsm.States.Add(AwaitingPlayerInput);
	Fsm.ChangeState("OnAwaitingPlayerInput", false);


	CasaState* OnEnemyTurn = new CasaState();
	OnEnemyTurn->Name = "OnEnemyTurn";

	OnEnemyTurn->SetUpdateDelegate(this, &UGameManager::OnEnemyTurn);
	Fsm.States.Add(OnEnemyTurn);


	CasaState* DeathState = new CasaState();
	DeathState->Name = "OnDeath";

	DeathState->SetStartDelegate(this, &UGameManager::BlockPlayerInput);
	DeathState->SetUpdateDelegate(this, &UGameManager::OnPlayerDeath);
	Fsm.States.Add(DeathState);


}


// Called every frame
void UGameManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Fsm.CurrentState->FTickDelegate.ExecuteIfBound();
}

void UGameManager::OnEnemyTurn()
{
	for (AEnemyActor* Enemy : Enemies)
	{
		Enemy->Update();
	}

	Player->TurnFinished = false;

	Fsm.ChangeState("OnAwaitingPlayerInput");
}

void UGameManager::OnAwaitPlayerInput()
{
	if (Player->TurnFinished)
	{
		Fsm.ChangeState("OnEnemyTurn");
	}
}

void UGameManager::OnPlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm dead skull emoji"));
}

