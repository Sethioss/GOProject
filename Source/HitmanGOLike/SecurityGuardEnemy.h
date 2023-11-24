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
	virtual void PatrolStep() override;
	virtual void AlertedStart() override;
	virtual void AlertedStep() override;
	virtual void AlertedEnd() override;
	virtual void Attack() override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* mesh = nullptr;
};
