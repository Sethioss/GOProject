// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "ArmySoldierEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HITMANGOLIKE_API AArmySoldierEnemy : public AEnemyActor
{
	GENERATED_BODY()

public:
	AArmySoldierEnemy();

protected:

	virtual void BeginPlay() override;
	virtual void MoveToDestination() override;
	virtual void InitFsm() override;
	
	virtual void Init() override;
	virtual void OnStartGame() override;
	virtual void OnPreTurn() override;
	virtual void OnTurn() override;
	virtual void OnPostTurn() override;

	virtual void OnPreAttack() override;

	bool IsDeadEnd(class APathActor* Node);

	UPROPERTY(VisibleAnywhere)
	TArray<class APathActor*> CurrentPath;

};
