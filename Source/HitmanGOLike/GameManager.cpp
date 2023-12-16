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

//void UGameManager::UnregisterAllHostages()
//{
//	for (int i = 0; i < Instance->Enemies.Num(); ++i)
//	{
//		Instance->Enemies[i]->IsLookingForHostage = false;
//	}
//}

// Called when the game starts
void UGameManager::BeginPlay()
{
	Super::BeginPlay();

	//if (Instance)
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

	InitFsm();

	// ...
	TArray<AActor*> EnemiesToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActor::StaticClass(), EnemiesToFind);

	for (int i = 0; i < EnemiesToFind.Num(); ++i)
	{
		Cast<AEnemyActor>(EnemiesToFind[i])->Init();
		Cast<AEnemyActor>(EnemiesToFind[i])->Update();
	}

	TArray<AActor*> paths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathActor::StaticClass(), paths);

	for (int i = 0; i < paths.Num(); ++i)
	{
		for (int j = 0; j < Cast<APathActor>(paths[i])->ConnectorInfo.Num(); ++j)
		{
			if (Cast<APathActor>(paths[i])->ConnectorInfo[j].Direction == EGeneralDirectionEnum::VALNUM || Cast<APathActor>(paths[i])->ConnectorInfo[j].DestinationNode == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Error on node %s. One or multiple connectors have VALNUM direction, or has a nullptr destination node!"), *Cast<APathActor>(paths[i])->GetActorNameOrLabel());
			}
		}
	}
}

void UGameManager::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Instance->RemoveFromRoot();
}

void UGameManager::ResetAllPathWeights()
{
	for (int i = 0; i < Paths.Num(); ++i)
	{
		Paths[i]->FScore = 1;
	}
}

void UGameManager::InitFsm()
{
	Instance->Fsm = new CasaFSM();
	CasaState* InitGame = new CasaState();
	InitGame->Name = "OnInitiatingGame";
	InitGame->SetUpdateDelegate(this, &UGameManager::OnInitGame);
	Instance->Fsm->States.Add(InitGame);
	Instance->Fsm->ChangeState("OnInitiatingGame", false);

	CasaState* AwaitingPlayerInput = new CasaState();
	AwaitingPlayerInput->Name = "OnAwaitingPlayerInput";
	AwaitingPlayerInput->SetStartDelegate(this, &UGameManager::ReleasePlayerInput);
	AwaitingPlayerInput->SetUpdateDelegate(this, &UGameManager::OnAwaitPlayerInput);
	AwaitingPlayerInput->SetEndDelegate(this, &UGameManager::BlockPlayerInput);
	Instance->Fsm->States.Add(AwaitingPlayerInput);

	CasaState* PrePlayerTurnState = new CasaState();
	PrePlayerTurnState->Name = "OnPrePlayerTurn";
	PrePlayerTurnState->SetUpdateDelegate(this, &UGameManager::OnPrePlayerTurn);
	Instance->Fsm->States.Add(PrePlayerTurnState);

	CasaState* PlayerRotationState = new CasaState();
	PlayerRotationState->Name = "OnPlayerRotation";
	PlayerRotationState->SetUpdateDelegate(this, &UGameManager::OnPlayerRotation);
	Instance->Fsm->States.Add(PlayerRotationState);

	CasaState* PlayerTranslationState = new CasaState();
	PlayerTranslationState->Name = "OnPlayerTranslation";
	PlayerTranslationState->SetUpdateDelegate(this, &UGameManager::OnPlayerTranslation);
	Instance->Fsm->States.Add(PlayerTranslationState);

	CasaState* PostPlayerTurnState = new CasaState();
	PostPlayerTurnState->Name = "OnPostPlayerTurn";
	PostPlayerTurnState->SetUpdateDelegate(this, &UGameManager::OnPostPlayerTurn);
	Instance->Fsm->States.Add(PostPlayerTurnState);

	CasaState* PlayerMoveToDeath = new CasaState();
	PlayerMoveToDeath->Name = "OnPlayerMoveToDeath";
	PlayerMoveToDeath->SetUpdateDelegate(this, &UGameManager::OnPlayerMoveToDeath);
	Instance->Fsm->States.Add(PlayerMoveToDeath);

	CasaState* OnEnemyTurn = new CasaState();
	OnEnemyTurn->Name = "OnEnemyTurn";
	OnEnemyTurn->SetStartDelegate(this, &UGameManager::OnStartEnemyTurn);
	OnEnemyTurn->SetUpdateDelegate(this, &UGameManager::OnEnemyTurn);
	Instance->Fsm->States.Add(OnEnemyTurn);

	CasaState* EnemyAttack = new CasaState();
	EnemyAttack->Name = "OnEnemyAttack";
	EnemyAttack->SetUpdateDelegate(this, &UGameManager::OnEnemyAttack);
	Instance->Fsm->States.Add(EnemyAttack);

	CasaState* DeathState = new CasaState();
	DeathState->Name = "OnDeath";
	DeathState->SetStartDelegate(this, &UGameManager::BlockPlayerInput);
	DeathState->SetUpdateDelegate(this, &UGameManager::OnPlayerDeath);
	Instance->Fsm->States.Add(DeathState);

	CasaState* GameFailedState = new CasaState();
	GameFailedState->Name = "OnGameFailed";
	GameFailedState->SetUpdateDelegate(this, &UGameManager::OnGameFailed);
	Instance->Fsm->States.Add(GameFailedState);

	CasaState* GameSucceeded = new CasaState();
	GameSucceeded->Name = "OnGameSucceeded";
	GameSucceeded->SetUpdateDelegate(this, &UGameManager::OnGameSucceeded);
	Instance->Fsm->States.Add(GameSucceeded);

	CasaState* GameReward = new CasaState();
	GameReward->Name = "OnGameReward";
	GameReward->SetUpdateDelegate(this, &UGameManager::OnGameReward);
	Instance->Fsm->States.Add(GameReward);

	CasaState* GameRestart = new CasaState();
	GameRestart->Name = "OnGameRestart";
	GameRestart->SetUpdateDelegate(this, &UGameManager::OnGameRestart);
	Instance->Fsm->States.Add(GameRestart);

	CasaState* GameQuit = new CasaState();
	GameQuit->Name = "OnGameQuit";
	GameQuit->SetUpdateDelegate(this, &UGameManager::OnGameQuit);
	Instance->Fsm->States.Add(GameQuit);

	CasaState* GameNextLevel = new CasaState();
	GameNextLevel->Name = "OnGameNextLevel";
	GameNextLevel->SetUpdateDelegate(this, &UGameManager::OnGameNextLevel);
	Instance->Fsm->States.Add(GameNextLevel);

	CasaState* GamePause = new CasaState();
	GamePause->Name = "OnGamePause";
	GamePause->SetUpdateDelegate(this, &UGameManager::OnGamePause);
	Instance->Fsm->States.Add(GamePause);
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

void UGameManager::RegisterToBarrier(AActor* Act)
{
	FSMBarrier::BarrieredObjects.Add(Act);
}

void UGameManager::ReleaseFromBarrier(AActor* Act)
{
	if (FSMBarrier::BarrieredObjects.Contains(Act))
	{
		FSMBarrier::BarrieredObjects.Remove(Act);
	}
}

void UGameManager::OnInitGame()
{
	if (IsFSMBarrierEmpty())
	{
		Instance->Fsm->ChangeState("OnAwaitingPlayerInput");
	}
}

void UGameManager::OnAwaitPlayerInput()
{
	if (Instance->Player->TurnFinished)
	{
		Instance->Fsm->ChangeState("OnEnemyTurn");
	}
}

void UGameManager::OnPrePlayerTurn()
{

}

void UGameManager::OnPlayerRotation() {}
void UGameManager::OnPlayerTranslation() {}
void UGameManager::OnPlayerMoveToDeath() {}

void UGameManager::OnPostPlayerTurn() {}

void UGameManager::OnEnemyAttack(){}

void UGameManager::BlockPlayerInput()
{
	Instance->Player->TurnFinished = true;
}

void UGameManager::ReleasePlayerInput()
{
	Instance->Player->TurnFinished = false;
}

void UGameManager::OnStartEnemyTurn()
{
	for (AEnemyActor* Enemy : Instance->Enemies)
	{
		Enemy->AllowedToMove = true;
		RegisterToBarrier(Enemy);
	}
}

void UGameManager::OnEnemyTurn()
{
	for (AEnemyActor* Enemy : Instance->Enemies)
	{
		Enemy->Update();
	}

	if (IsFSMBarrierEmpty())
	{
		Instance->Fsm->ChangeState("OnAwaitingPlayerInput");
	}
}

void UGameManager::OnPlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm dead skull emoji"));
}

void UGameManager::OnSaveHostage()
{
	for (int i = 0; i < Instance->Enemies.Num(); ++i)
	{
		Enemies[i]->IsLookingForHostage = false;
	}
}

void UGameManager::OnGameSucceeded(){}
void UGameManager::OnGameFailed(){}
void UGameManager::OnGameRestart(){}
void UGameManager::OnGameReward(){}
void UGameManager::OnGameQuit(){}
void UGameManager::OnGameNextLevel(){}

void UGameManager::OnGamePause() {}