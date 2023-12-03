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


	virtual void NeutralTurn() override;
	TArray<APathActor*> GetPathTo(APathActor* Dest);

	virtual void GetDestination() override;
	virtual void MoveToDestination() override;

	virtual void InitFsm() override;
	
	UPROPERTY(VisibleAnywhere)
	TArray<APathActor*> CurrentPath;

	APathActor* GetDestFromPath(TArray<APathActor*> Path);
};
