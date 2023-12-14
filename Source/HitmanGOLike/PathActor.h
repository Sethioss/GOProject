// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CasaPlayer.h"
#include "PathActor.generated.h"

/*
* Classe des Chemins
* Un chemin peut �tre une simple jonction ou un Node o� le Joueur pourra se d�placer.
*/

UCLASS()
class HITMANGOLIKE_API APathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathActor();
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PlaneMesh = nullptr;
	//R�ference vers le Joueur
	UPROPERTY()
	class ACasaPlayer* PlayerPawn;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Indique si le Joueur est sur un Node voisin du Node actuel
	bool IsPlayerOnNeighbouringNode();
	void TransferPlayerOwnership(APathActor& OriginTile);
	//Indique si la Foreuse est sur un Node voisin du Node actuel
	APathActor* IsForeuseOnNeighbourinNode();
	UFUNCTION(BlueprintCallable)
	inline bool GetIsNode() { return IsNode; }
	UFUNCTION(BlueprintCallable)
	inline TArray<APathActor*> GetNeighbouringNodes() { return NeighbouringNodes; }
	//R�cup�re tout les Nodes voisin de la Node
	UFUNCTION()
	TArray<APathActor*> ReachNeighbouringPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Le Chemin est un noeud ou non
	UPROPERTY(EditAnywhere, Category = NodePathParameters)
	bool IsNode = false;
	//Noeuds voisins
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	TArray<APathActor*> NeighbouringNodes;
	//Point de d�part ou non
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool StartingNode = false;
	//Point d'arriv� ou non
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool EndingNode = false;
	//Ajoute les noeuds voisins � NeighbouringNodes
	UFUNCTION()
	void AddNeighbouringNodes();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

};
