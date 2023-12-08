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

	void UnregisterVisitedNodesUntilLastCheckpoint(TArray<FConnectorInfo*> TempPath, TArray<int> Checkpoints);
	void AStarAlgorithm(APathActor* Start, APathActor* End);
	void GetBestPath(FConnectorInfo* Start, APathActor* End, TArray<FConnectorInfo*> TempPath, TArray<int> Checkpoints, EGeneralDirectionEnum& TempDir, int& AnglesNumber);
	bool IsDeadEnd(APathActor* Node);


	
	UPROPERTY(VisibleAnywhere)
	TArray<APathActor*> CurrentPath;
	TArray<APathActor*> VisitedNodes;

	float ManhattanDistance(FVector A, FVector B);
	TArray<APathActor*> BestPath;
};
