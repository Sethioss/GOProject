// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Otage.generated.h"

/**
 * 
 */
UCLASS()
class HITMANGOLIKE_API AOtage : public AItem
{
	GENERATED_BODY()
	
public:
	void SetOtageLocation(APathActor* Target);
	FBox PlacingArea;
	bool Placable=true;


	void BeginPlay();
	//Call the special effect of the item
	virtual void ItemEffect() override;
	virtual void Tick(float DeltaTime) override;

protected:

private:

};
