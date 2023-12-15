// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CasaPlayer.h"
#include "PathActor.generated.h"

/*
* Classe des Chemins
* Un chemin peut être une simple jonction ou un Node où le Joueur pourra se déplacer.
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
	//Réference vers le Joueur
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
	//Récupère tout les Nodes voisin de la Node
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
	//Point de départ ou non
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool StartingNode = false;
	//Point d'arrivé ou non
	UPROPERTY(EditAnywhere, Category = NodeInfo)
	bool EndingNode = false;
	//Ajoute les noeuds voisins à NeighbouringNodes
	UFUNCTION()
	void AddNeighbouringNodes();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

};
