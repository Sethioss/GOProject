// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "PathActor.h"
#include "SecurityGuardEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HITMANGOLIKE_API ASecurityGuardEnemy : public AEnemyActor
{
	GENERATED_BODY()
public:
	ASecurityGuardEnemy();

protected:

	virtual void BeginPlay() override;

	virtual void OnPreTurn() override;
	virtual void OnTurn() override;
	virtual void OnPostTurn() override;

	virtual APathActor* GetDestination() override;
	virtual void MoveToDestination() override;

	virtual void InitFsm() override;
};
