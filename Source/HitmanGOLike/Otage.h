// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Otage.generated.h"

/**
 * Classe Otage.
 * Il peut être "lancé" à 2 cases de distance.
 * Son cri alerte les ennemies à 1 case de distance.
 */

UCLASS()
class HITMANGOLIKE_API AOtage : public AItem
{
	GENERATED_BODY()
	
public:
	void SetOtageLocation(APathActor* Target);
	//Zone de placement
	FBox PlacingArea;
	//L'Otage est placable ou non
	bool Placable=true;
	//Définit les valeur par défaut fonction de l'emplacement de l'otage.
	void BeginPlay();
	//Call the special effect of the item
	virtual void ItemEffect() override;
	virtual void Tick(float DeltaTime) override;

};
