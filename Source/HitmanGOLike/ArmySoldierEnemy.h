// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "PathActor.h"
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
	virtual APathActor* GetDestination() override;
	virtual void MoveToDestination() override;
	virtual void InitFsm() override;
	virtual void NeutralTurn() override;

	void UnregisterVisitedNodesUntilLastCheckpoint(TArray<APathActor*> TempPath, TArray<int> Checkpoints);
	void GetBestPathFromConnectors(APathActor* Start, APathActor* End, TArray<APathActor*> TempPath, TArray<int> Checkpoints);
	void GetBestPath(APathActor* Start, APathActor* End, TArray<APathActor*> TempPath, TArray<int> Checkpoints);
	bool IsDeadEnd(APathActor* Node);


	
	UPROPERTY(VisibleAnywhere)
	TArray<APathActor*> CurrentPath;
	TArray<APathActor*> VisitedNodes;

	TArray<APathActor*> BestPath;
};
