// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CasaFSM.h"
#include "PathActor.h"
#include "EnemyActor.h"
#include "Otage.h"
#include "GameManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITMANGOLIKE_API UGameManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameManager();
	class ACasaPlayer* Player;
	UPROPERTY(VisibleAnywhere)
	TArray<class AEnemyActor*> Enemies;

	int ElementsToRegister = 0;
	TArray<APathActor*> Paths;

	//void UnregisterAllHostages();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	CasaFSM* Fsm;
	FSMBarrier* Barrier;
	CasaState* SavedState;

	void InitFsm();

	void OnInitGame();

	void OnAwaitPlayerInput();

	void OnPrePlayerTurn();

	void OnEnemyTurn();
	void OnPlayerDeath();
	void OnPlayerMoveToDeath();

	void OnPlayerRotation();
	void OnPlayerTranslation();
	void OnPostPlayerTurn();

	void OnStartEnemyTurn();
	void OnEnemyAttack();

	void OnGameFailed();
	void OnGameSucceeded();
	void OnGameRestart();
	void OnGameReward();
	void OnGameQuit();
	void OnGameNextLevel();

	void OnGamePause();

	void BlockPlayerInput();
	void ReleasePlayerInput();
	void OnSaveHostage();

	static UGameManager* Instance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category= "GameManager singleton")
	static UGameManager* GetInstance();

	void ResetAllPathWeights();
	void UnregisterAllHostages();

	APathActor* GetPlayerNode();

	void KillPlayer();
	void ClearBarrier();
	void RegisterToBarrier(AActor* Act);
	void ReleaseFromBarrier(AActor* Act);
	inline bool IsFSMBarrierEmpty() { return FSMBarrier::BarrieredObjects.Num() == 0; }

		
};
