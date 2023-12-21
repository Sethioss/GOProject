// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CasaFSM.h"
#include "PathActor.h"
#include "EnemyActor.h"
#include "Otage.h"
#include "Foreuse.h"
#include "Containers/Map.h"
#include "GameManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITMANGOLIKE_API UGameManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere)
	TArray<class AActor*> Cameras;

	bool JustChangedSubLevel = false;

	// Sets default values for this component's properties
	UGameManager();
	class ACasaPlayer* Player;
	UPROPERTY(VisibleAnywhere)
	TArray<class AEnemyActor*> Enemies;

	int ElementsToRegister = 0;
	TArray<APathActor*> Paths;
	TArray<AItem*> Items;
	TArray<AWall*> Walls;

	UPROPERTY(EditAnywhere)
	TArray<APathActor*> SubLevelStartPaths;

	bool CheckIfWall(APathActor* Node1, APathActor* Node2, bool CheckIfBroken);

	void SetClock();
	float Clock = 3;

	UPROPERTY(EditAnywhere)
	FName NextLevel;

	inline CasaFSM* GetFsm() { return Instance->Fsm; }

	APathActor* PlayerNextNode;

	void InitPlayer(ACasaPlayer* pl);


	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int FirstLevelID = 1;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, class USoundWave*> AudioData;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	CasaFSM* Fsm;
	CasaState* SavedState;
	FSMBarrier* Barrier;

	void InitFsm();

	void OnInitGame();
	void OnStartGame();

	void OnAwaitPlayerInput();

	void OnPrePlayerTurn();
	void OnPlayerTurn();
	void OnPlayerMoveWithDrill();

	void OnEnemyTurn();
	void OnPlayerDeath();
	void OnPlayerMoveToDeath();

	void OnPlayerRotation();
	void OnPlayerTranslation();
	void OnPostPlayerTurn();

	void OnStartEnemyTurn();
	void OnEnemyAttack();

	void OnPostGameTurn();

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

	void InitiateGameDataFromCasaInstance();
	void InitiateSceneDataFromCasaInstance();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category= "GameManager singleton")
	static UGameManager* GetInstance();

	void ResetAllPathWeights();
	void UnregisterHostage(AOtage* Otage);

	APathActor* GetPlayerNode();

	void KillPlayer();
	void ClearBarrier();
	void RegisterToBarrier(AActor* Act);
	void ReleaseFromBarrier(AActor* Act);
	inline bool IsFSMBarrierEmpty() { return FSMBarrier::BarrieredObjects.Num() == 0; }

	void PlaySound(FString const AudioName );

		
};
