// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "CasaFSM.h"
#include "EnemyActor.generated.h"

UENUM(BlueprintType)
enum class EEnemyDirectionEnum : uint8 {
	UP = 0 UMETA(DisplayName = "UP"),
	RIGHT = 1 UMETA(DisplayName = "RIGHT"),
	DOWN = 2 UMETA(DisplayName = "DOWN"),
	LEFT = 3 UMETA(DisplayName = "LEFT"),
	FORWARDS = 4 UMETA(DisplayName = "FORWARDS"),
	LOCALRIGHT = 5 UMETA(DisplayName = "LOCALRIGHT"),
	BACKWARDS = 6 UMETA(DisplayName = "BACKWARDS"),
	LOCALLEFT = 7 UMETA(DisplayName = "LOCALLEFT"),
	VALNUM = 8,
};

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
	virtual APathActor* GetNodeAtCardinalDirection(EEnemyDirectionEnum Dir);
	virtual FVector GetNormalizedVectorFromDirection(EEnemyDirectionEnum Dir);

	virtual APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0)) override;

	bool TurnFinished = false;

	void RegisterToManager();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

