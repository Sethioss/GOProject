// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Directionality.h"
#include "PathActor.generated.h"

USTRUCT(BlueprintType)
struct FConnectorInfo
{
	GENERATED_USTRUCT_BODY();

public:
	/** Default constructor */
	FConnectorInfo() {};

public:

	inline bool operator==(const FConnectorInfo& other) const
	{
		return (other.Direction == Direction && other.DestinationNode == DestinationNode);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGeneralDirectionEnum Direction;

	/** Intensity of yaw input as modifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APathActor* DestinationNode;
};

UCLASS()
class HITMANGOLIKE_API APathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathActor();

	//Put this in a manager
	bool IsConnectedNode(APathActor* A, APathActor* B);
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif

	UPROPERTY(EditAnywhere, Category = NodeInfo)
	TArray<FConnectorInfo> ConnectorInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool StartingNode = false;
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool EndingNode = false;
	UPROPERTY()
	class ACasaPlayer* PlayerPawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PlaneMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Up = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Down = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Right = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Left = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters")
	bool IsWalkableNode = true;

	APathActor* CheckNeighbourNode(int Direction, bool GetConnected = false);
	void SetMaterialBoolParameterValue(UMaterialInstanceDynamic* DynMat, bool& boolVal, FString boolValName, float value);
	void RemoveConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction);
	void AddConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction);
	bool CheckConnectorInfo(APathActor* CurNode, EGeneralDirectionEnum Direction);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsPlayerOnNeighbouringNode();
	void TransferPlayerOwnership(APathActor& OriginTile);

	inline UStaticMeshComponent* GetPlaneMesh() { return PlaneMesh; }


	UFUNCTION(BlueprintCallable)
	inline bool GetIsNode(){ return IsWalkableNode; }

	bool Visited = false;

};
