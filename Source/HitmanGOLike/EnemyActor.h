// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "CasaFSM.h"
#include "Directionality.h"
#include "Otage.h"
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

	USceneComponent* Holder;

	UPROPERTY(VisibleAnywhere)
	APathActor* CurrentNode;

	UPROPERTY(VisibleAnywhere)
	APathActor* Destination;

	void Alert(AOtage* Otage, bool GetStunned);

	virtual void Update();
	virtual void Init();

	UPROPERTY(VisibleAnywhere)
	bool IsLookingForHostage = false;
	UPROPERTY(VisibleAnywhere)
	AOtage* Hostage;

	bool RegisteredToManager = false;

	CasaFSM* Fsm;
	bool AllowedToMove = false;

	void ClearBestPath();

	bool Stunned = false;
	bool ReadyToSaveHostage = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();
	virtual void InitFsm();

	virtual APathActor* GetDestination();
	virtual void MoveToDestination();
	virtual void MoveToHalfDestination();
	void RemoveRightToMove();

	virtual APathActor* GetCurrentNode();
	virtual APathActor* GetNodeAtCardinalDirection(EGeneralDirectionEnum Dir, bool GetConnected);
	virtual FVector GetNormalizedVectorFromDirection(EGeneralDirectionEnum Dir);

	virtual APathActor* SnapToGrid(FVector offset) override;

	virtual APathActor* GetDestinationByPathfinding(APathActor* Target);
	virtual TArray<APathActor*> AStarAlgorithm(APathActor* Target, APathActor* End, TArray<APathActor*>BlacklistedNodes);

	bool TurnFinished = false;

	virtual void OnRegisterToManager();
	virtual void OnStartGame();
	virtual void OnAwait();

	virtual void OnStandby();

	virtual void OnPreTurn();
	virtual void OnTurn();
	virtual void OnPostTurn();

	virtual void OnPreAttack();
	virtual void OnAttack();
	virtual void OnPostAttack();


	float ManhattanDistance(FVector A, FVector B);
	UPROPERTY(VisibleAnywhere)
	TArray<APathActor*> BestPath;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

