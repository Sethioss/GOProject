// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Foreuse.generated.h"

/**
 * 
 */
UCLASS()
class HITMANGOLIKE_API AForeuse : public AItem
{
	GENERATED_BODY()

protected:
	//Call the special effect of the item
	virtual void ItemEffect() override;

public:
	

	UFUNCTION()
	void Init(APathActor* Node) { CurrentNode = Node; }
	
	AForeuse(APathActor* CurrentNode);
	AForeuse();

	void SetForeuseLocation(APathActor* NewNode, FVector NodePos);

};
