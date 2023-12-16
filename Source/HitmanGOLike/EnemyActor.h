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

	UPROPERTY(VisibleAnywhere)
	APathActor* CurrentNode;

	UPROPERTY(VisibleAnywhere)
	APathActor* Destination;

	void Alert(AOtage* Otage);

	virtual void Update();
	void Init();

	UPROPERTY(VisibleAnywhere)
	bool IsLookingForHostage = false;
	UPROPERTY(VisibleAnywhere)
	AOtage* Hostage;

	bool RegisteredToManager = false;

	CasaFSM* Fsm;
	bool AllowedToMove = false;

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

	virtual APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0)) override;

	virtual APathActor* GetDestinationByPathfinding(APathActor* Target);
	virtual TArray<APathActor*> AStarAlgorithm(APathActor* Target, APathActor* End, TArray<APathActor*>BlacklistedNodes);

	bool TurnFinished = false;

	void OnRegisterToManager();
	virtual void OnAwait();

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

