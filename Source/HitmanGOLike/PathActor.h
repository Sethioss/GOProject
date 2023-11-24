// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.generated.h"

UCLASS()
class HITMANGOLIKE_API APathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PlaneMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = NodePathParameters)
	bool IsNode = false;
	UPROPERTY(EditAnywhere, Category = NodePathParameters)
	bool SpawnForeuse = false;
	UPROPERTY(EditAnywhere, Category = NodePathParameters)
	bool SpawnOtage = false;

	UPROPERTY(EditAnywhere, Category = NodeInfo)
	TArray<APathActor*> NeighbouringNodes;
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool StartingNode = false;
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool EndingNode = false;
	UPROPERTY()
	class ACasaPlayer* PlayerPawn;
	UFUNCTION()
	void AddNeighbouringNodes();



	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsPlayerOnNeighbouringNode();
	void TransferPlayerOwnership(APathActor& OriginTile);

	UFUNCTION(BlueprintCallable)
	inline bool GetIsNode(){ return IsNode; }
	UFUNCTION(BlueprintCallable)
	inline TArray<APathActor*> GetNeighbouringNodes() { return NeighbouringNodes; }
	UFUNCTION()
	TArray<APathActor*> ReachNeighbouringPath();

};