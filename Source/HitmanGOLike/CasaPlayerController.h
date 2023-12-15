// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CasaPlayer.h"
#include "GameFramework/PlayerController.h"
#include "CasaPlayerController.generated.h"

/*
* Classe du Controller du Joueur.
* Il permet de gérer les clics et leur impact sur les objets de la scène:
*	- Deplacement avec/sans objet
*	- Équipement d'objet
*	- Utilisation d'objet
*	- Définit les règles relatives au GameDesign des objets de la scène (validité des actions effectuées)
*/

UCLASS()
class HITMANGOLIKE_API ACasaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//Set Default Values
	ACasaPlayerController();

	UFUNCTION()
	void OnMouseClick();
	UFUNCTION()
	void SetupInputComponent();
	//Désequipe l'objet tenu par le joueur
	UFUNCTION()
	void OnPressF();
	//Référence vers le Joueur
	UPROPERTY(EditDefaultsOnly, Category=Player)
	TSubclassOf<ACasaPlayer> CasaPlayer;

};
