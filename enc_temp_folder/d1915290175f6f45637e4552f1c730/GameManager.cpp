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
	
	for (int i = 0; i < EnemiesToFind.Num(); ++i)
	{
		Enemies.Add(Cast<AEnemyActor>(Enemies[i]));
		//Cast<AEnemyActor>(EnemiesToFind[i])->Init();
	}
	//Instance->ElementsToRegister = EnemiesToFind.Num();

	InitFsm();
}

void UGameManager::InitGame() 
{
	Instance->Fsm->SetNextState("OnAwaitingPlayerInput");

	UE_LOG(LogTemp, Warning, TEXT("I Init"));

	for (int i = 0; i < Enemies.Num(); ++i)
	{
		//Enemies.Add(Enemies[i]);
		//Cast<AEnemyActor>(Enemies[i])->Update();
	}

	if (IsFSMBarrierEmpty())
	{
		Instance->Fsm->GoToNextState();
	}
}

void UGameManager::OnAwaitPlayerInput()
{
	UE_LOG(LogTemp, Warning, TEXT("I wait for input"));
	Instance->Fsm->SetNextState("OnEnemyTurn");

	if (Instance->Player->TurnFinished)
	{
		Instance->Fsm->GoToNextState();
	}
}

void UGameManager::RegisterToBarrier(AActor* Act)
{
	FSMBarrier::BarrieredObjects.Add(Act);
}

void UGameManager::ReleaseFromBarrier(AActor* Act)
{
	if (FSMBarrier::BarrieredObjects.Contains(Act))
	{
		FSMBarrier::BarrieredObjects.Remove(Act);
		UE_LOG(LogTemp, Warning, TEXT("Releasing %s from barrier. New barrier length: %i"), *Act->GetActorNameOrLabel(), FSMBarrier::BarrieredObjects.Num());
	}
}

void UGameManager::BlockPlayerInput()
{

}

void UGameManager::ReleasePlayerInput()
{

}

void UGameManager::ResetAllPathWeights()
{
	for (int i = 0; i < Paths.Num(); ++i)
	{
		Paths[i]->FScore = 1;
	}
}

void UGameManager::StartEnemyTurn()
{
	for (AEnemyActor* Enemy : Instance->Enemies)
	{
		Enemy->AllowedToMove = true;
	}
}

void UGameManager::OnEnemyTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm doing the enemy turn"));
	for (AEnemyActor* Enemy : Instance->Enemies)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy: %s - Current State: %s"), *Enemy->GetActorNameOrLabel(), *Enemy->Fsm->CurrentState->Name);
		RegisterToBarrier(Enemy);
		Enemy->Update();
	}

	if (IsFSMBarrierEmpty())
	{
		Instance->Player->TurnFinished = false;

		Instance->Fsm->ChangeState("OnAwaitingPlayerInput");
	}
}

void UGameManager::OnPlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm dead skull emoji"));
}

void UGameManager::InitFsm()
{
	Instance->Fsm = new CasaFSM();
	CasaState* InitGame = new CasaState();
	InitGame->Name = "InitiatingGame";
	InitGame->SetUpdateDelegate(this, &UGameManager::InitGame);
	Instance->Fsm->States.Add(InitGame);
	Instance->Fsm->ChangeState("InitiatingGame", false);

	CasaState* AwaitingPlayerInput = new CasaState();
	AwaitingPlayerInput->Name = "OnAwaitingPlayerInput";
	AwaitingPlayerInput->SetStartDelegate(this, &UGameManager::ReleasePlayerInput);
	AwaitingPlayerInput->SetUpdateDelegate(this, &UGameManager::OnAwaitPlayerInput);
	AwaitingPlayerInput->SetEndDelegate(this, &UGameManager::BlockPlayerInput);
	Instance->Fsm->States.Add(AwaitingPlayerInput);
	Instance->Fsm->SetNextState("OnAwaitingPlayerInput");


	CasaState* OnEnemyTurn = new CasaState();
	OnEnemyTurn->Name = "OnEnemyTurn";
	OnEnemyTurn->SetStartDelegate(this, &UGameManager::StartEnemyTurn);
	OnEnemyTurn->SetUpdateDelegate(this, &UGameManager::OnEnemyTurn);
	Instance->Fsm->States.Add(OnEnemyTurn);


	CasaState* DeathState = new CasaState();
	DeathState->Name = "OnDeath";
	DeathState->SetStartDelegate(this, &UGameManager::BlockPlayerInput);
	DeathState->SetUpdateDelegate(this, &UGameManager::OnPlayerDeath);
	Instance->Fsm->States.Add(DeathState);
}


// Called every frame
void UGameManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Instance->Fsm->CurrentState->FTickDelegate.Execute();
}

UGameManager* UGameManager::GetInstance()
{
	return Instance;
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
