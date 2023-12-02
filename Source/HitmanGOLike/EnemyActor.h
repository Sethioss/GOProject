// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "CasaFSM.h"
#include "EnemyActor.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	NEUTRAL = 0 UMETA(DisplayName = "NEUTRAL"),
	ALERTED = 1 UMETA(DisplayName = "ALERTED"),
	SEARCHING = 2 UMETA(DisplayName = "SEARCHING"),
	ATTACKING = 3 UMETA(DisplayName = "ATTACKING"),
	DEAD = 4 UMETA(DisplayName = "DEAD"),
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

	UPROPERTY(VisibleAnywhere)
	EEnemyState State;

	virtual void Update();

	CasaFSM Fsm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitFsm();

	virtual void NeutralTurn();

	virtual void GetDestination();
	virtual void MoveToDestination();

	virtual void Attack();



	virtual APathActor* GetCurrentNode();

	virtual APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0)) override;

	bool TurnFinished = false;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

