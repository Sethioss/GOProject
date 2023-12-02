// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CasaFSM.h"
#include "GameManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITMANGOLIKE_API UGameManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TArray<class AEnemyActor*> Enemies;

	UPROPERTY(VisibleAnywhere)
	class ACasaPlayer* Player;
	class CasaFSM Fsm;

	void InitFsm();

	void OnAwaitPlayerInput();
	void OnEnemyTurn();
	void OnPlayerDeath();

	void BlockPlayerInput();
	void ReleasePlayerInput();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
