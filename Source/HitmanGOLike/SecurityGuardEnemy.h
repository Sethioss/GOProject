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
	virtual void UpdateBehaviour() override;
	virtual void BeginTurnBehaviour() override;

protected:
	
	virtual void BeginPlay() override;
	virtual void PatrolStep() override;
	virtual void AlertedStart() override;
	virtual void AlertedStep() override;
	virtual void AlertedEnd() override;
	virtual void Attack() override;

	virtual void MoveToDestination() override;

	virtual void InitFsm() override;
};
