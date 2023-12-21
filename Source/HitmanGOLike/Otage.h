// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Otage.generated.h"

/**
 * Classe Otage.
 * Il peut �tre "lanc�" � 2 cases de distance.
 * Son cri alerte les ennemies � 1 case de distance.
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

	UPROPERTY(EditAnywhere)
	int SubLevel = 0;
	//D�finit les valeur par d�faut fonction de l'emplacement de l'otage.
	virtual void BeginPlay() override;
	//Call the special effect of the item
	//virtual void ItemEffect() override;
	virtual void Tick(float DeltaTime) override;

	void ItemEffect(bool StunEnemies);

	void DropHostage();
	void LiftHostage();

};
