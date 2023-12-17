// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "PathActor.h"
#include "IBoardElement.h"
#include "Item.generated.h"

/*
* Classe Item.
* Definit les comportements et paramètres commun a tout les items:
*	- S'il sont tenu
*	- Mesh
*/

UCLASS()
class HITMANGOLIKE_API AItem : public AActor, public IBoardElement
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem(APathActor* CurrentNode);
	AItem();
	void SetIsHeld();
	APathActor* GetCurrentNode() { return CurrentNode; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Node où se situe l'objet
	UPROPERTY(EditAnywhere)
	APathActor* CurrentNode = nullptr;

	APathActor* NextNode = nullptr;

	APathActor* SnapToGrid(FVector offset = FVector(0, 0, 0));

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY()
	class UAudioComponent* AudioComponent = nullptr;

	//Node en face de l'item permet d'utiliser l'item (avancer la foreuse, pousser l'otage)
	//UFUNCTION()
	//APathActor* IsForwardNodeValid();
	//Node derrière l'item permet d'utiliser l'item (uniquement pour la foreuse -> la reculer)
	UFUNCTION()
	APathActor* IsBackwardNodeValid();
	//Call the special effect of the item

	bool IsHeld = false;

};
