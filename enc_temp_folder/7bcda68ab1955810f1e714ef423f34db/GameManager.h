// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CasaFSM.h"
#include "PathActor.h"
#include "EnemyActor.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	CasaFSM* Fsm;
	FSMBarrier* Barrier;

	void InitFsm();

	void OnAwaitPlayerInput();
	void OnEnemyTurn();
	void OnPlayerDeath();

	void StartEnemyTurn();

	void BlockPlayerInput();
	void ReleasePlayerInput();

	static UGameManager* Instance;

	void InitGame();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category= "GameManager singleton")
	static UGameManager* GetInstance();

	void ResetAllPathWeights();

	APathActor* GetPlayerNode();

	void RegisterToBarrier(AActor* Act);
	void ReleaseFromBarrier(AActor* Act);
	inline bool IsFSMBarrierEmpty() { return FSMBarrier::BarrieredObjects.Num() > 0; }

		
};
