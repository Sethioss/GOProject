// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "CasaFSM.h"
#include "Directionality.h"
#include "EnemyActor.generated.h"

UCLASS()
class HITMANGOLIKE_API AEnemyActor : public AActor, public IBoardElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyActor();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	APathActor* CurrentNode;

	UPROPERTY(VisibleAnywhere)
	APathActor* Destination;

	virtual void Update();

	bool RegisteredToManager = false;

	CasaFSM Fsm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();
	virtual void InitFsm();

	virtual void NeutralTurn();

	virtual APathActor* GetDestination();
	virtual void MoveToDestination();

	virtual void Attack();


	virtual APathActor* GetCurrentNode();
	virtual APathActor* GetNodeAtCardinalDirection(EGeneralDirectionEnum Dir, bool GetConnected = false);
	virtual FVector GetNormalizedVectorFromDirection(EGeneralDirectionEnum Dir);

	virtual APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0)) override;

	bool TurnFinished = false;

	void RegisterToManager();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

