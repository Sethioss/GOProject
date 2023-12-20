// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "Wall.generated.h"

/*
* Classe du Mur.
* Bloque le passage.
* Peut être cassé avec une Foreuse liberant le passage uniquement au Joueur
*/

UCLASS()
class HITMANGOLIKE_API AWall : public AActor, public IBoardElement
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();
	//Definit si le mur est cassé ou non
	bool IsBroken = false;
	//Casse le Mur, permet de le traverser
	void Break();

	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	APathActor* CurrentNode;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0));

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;

};
