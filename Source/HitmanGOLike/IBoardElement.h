// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBoardElement.generated.h"

UINTERFACE(MinimalAPI)
class UBoardElement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HITMANGOLIKE_API IBoardElement
{
	GENERATED_BODY()

public:
	virtual class APathActor* SnapToGrid(FVector offset) = 0;
	bool PathfindingObstacle = false;
	bool PlayerObstacle = true;
};
