// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Wall.h"
#include "Foreuse.generated.h"

/*
* Classe Foreuse.
* Objet permettant une fois équipée de bloquer des passages et de casser un Wall.
* Se situe sur la case adjacente au Joueur.
* Peut être déplacée de manière unilatérale.
*/

UCLASS()
class HITMANGOLIKE_API AForeuse : public AItem
{
	GENERATED_BODY()

public:
	//Call the special effect of the item
	virtual void ItemEffect(AWall* Wall);

	virtual void BeginPlay() override;
	
	//Set Default Value
	UFUNCTION()
	void Init(APathActor* Node) { CurrentNode = Node; }
	AForeuse(APathActor* CurrentNode);
	AForeuse();

	//Modifie la position de la Foreuse
	void SetForeuseLocation(APathActor* NewNode, FVector NodePos);
	//Definit si la Foreuse est utilisable ou non
	bool Usable = true;

	void Remove();

};
