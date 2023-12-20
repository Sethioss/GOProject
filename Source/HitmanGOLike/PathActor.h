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
	FConnectorInfo() { Direction = EGeneralDirectionEnum::VALNUM; };

public:

	inline bool operator==(const FConnectorInfo& other) const
	{
		return (other.Direction == Direction && other.DestinationNode == DestinationNode);
	}

	inline bool operator!=(const FConnectorInfo& other) const
	{
		return (other.Direction != Direction || other.DestinationNode != DestinationNode);
	}

	/** Intensity of yaw input as modifier */
	class APathActor* OriginNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGeneralDirectionEnum Direction;

	/** Intensity of yaw input as modifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APathActor* DestinationNode;
};

class ACasaPlayer;

UCLASS()
class HITMANGOLIKE_API APathActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsObstacle = false;
	// Sets default values for this actor's properties
	APathActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PlaneMesh = nullptr;
	
	UPROPERTY()
	class ACasaPlayer* PlayerPawn;

	UPROPERTY(EditAnywhere)
	bool PathSecondary = false;

	UPROPERTY(EditAnywhere)
	int LoadCamera = 0;
	
	//Put this in a manager
	bool IsConnectedNode(APathActor* A, APathActor* B);
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif

	UPROPERTY(EditAnywhere, Category = NodeInfo)
	TArray<FConnectorInfo> ConnectorInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters")
	bool IsWalkableNode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NodeInfo)
	float PartOfEnemyPath = 0;

	void SetIsEnemyPath(float value);

	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool StartingNode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NodeInfo)
	bool EndingNode = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, Category = NodePathParameters)
    bool IsNode = false;

	UPROPERTY(EditAnywhere, Category = NodeInfo)
    TArray<APathActor*> NeighbouringNodes;

	void AddMaterialFloatParameterValue(UMaterialInstanceDynamic* DynMat, float& floatVal, FString floatValName, float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Up = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Down = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Right = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodePathParameters | Connectors")
	bool Left = true;

	

	APathActor* CheckNeighbourNode(int Direction, bool GetConnected = false);
	APathActor* CheckNeighbourNode(EGeneralDirectionEnum Dir, bool GetConnected = false);
	void SetMaterialBoolParameterValue(UMaterialInstanceDynamic* DynMat, bool& boolVal, FString boolValName, float value);
	void RemoveConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction);
	void AddConnector(APathActor* CurNode, APathActor* DestNode, EGeneralDirectionEnum Direction);
	bool CheckConnectorInfo(APathActor* CurNode, EGeneralDirectionEnum Direction);
	double ManhattanDistance(FVector StartPos, FVector EndPos);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APathActor* IsPlayerOnNeighbouringNode();
	bool IsPlayerOnNeighbouringNodeWithoutOwnershipTransfer();
	void TransferPlayerOwnership(APathActor& OriginTile);
	
	bool HasObjectOnIt = false;
	APathActor* IsForeuseOnNeighbouringNode();

	inline UStaticMeshComponent* GetPlaneMesh() { return PlaneMesh; }


	UFUNCTION(BlueprintCallable)
	inline bool GetIsNode(){ return IsWalkableNode; }

    UFUNCTION(BlueprintCallable)
    inline TArray<APathActor*> GetNeighbouringNodes() { return NeighbouringNodes; }

	float FScore = 1;

	APathActor* PathfindingParent;

	bool Visited = false;

};
