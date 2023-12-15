// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CasaPlayer.h"
#include "GameFramework/PlayerController.h"
#include "CasaPlayerController.generated.h"

/*
* Classe du Controller du Joueur.
* Il permet de g�rer les clics et leur impact sur les objets de la sc�ne:
*	- Deplacement avec/sans objet
*	- �quipement d'objet
*	- Utilisation d'objet
*	- D�finit les r�gles relatives au GameDesign des objets de la sc�ne (validit� des actions effectu�es)
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
	//D�sequipe l'objet tenu par le joueur
	UFUNCTION()
	void OnPressF();
	//R�f�rence vers le Joueur
	UPROPERTY(EditDefaultsOnly, Category=Player)
	TSubclassOf<ACasaPlayer> CasaPlayer;

};
