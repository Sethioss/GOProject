// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CasaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HITMANGOLIKE_API UCasaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FString, class USoundWave*> AudioData;

	UPROPERTY(EditAnywhere)
	TMap<int, FString> LevelList;

	UPROPERTY(VisibleAnywhere)
	bool InitiatedGame;

	UPROPERTY(VisibleAnywhere)
	int CurrentLevelID;

	UPROPERTY(EditAnywhere)
	int CurrentSubLevel = 0;
	
};
