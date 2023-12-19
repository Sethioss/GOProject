// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CasaPlayer.h"
#include "GameFramework/PlayerController.h"
#include "CasaPlayerController.generated.h"

/**
 * 
 */
class GameManager;
UCLASS()
class HITMANGOLIKE_API ACasaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACasaPlayerController();

	UFUNCTION()
	void OnMouseClick();
	UFUNCTION()
	void SetupInputComponent();

	UFUNCTION()
	void OnPressF();

	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category=Player)
	TSubclassOf<ACasaPlayer> CasaPlayer;

	void OnReload();
	void OnLoadNextLevel();

protected:
	//virtual void SetupInputComponent() override;
};
