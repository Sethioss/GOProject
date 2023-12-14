// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CasaPlayer.generated.h"

/*
* Classe du Joueur.
* Il peut se deplacer en point and click sur les Noeud Uniquement.
* Tenir et Utiliser des objets.
*/

class AItem;

UCLASS()
class HITMANGOLIKE_API ACasaPlayer : public APawn
{
	GENERATED_BODY()


protected:
	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY()
	class UAudioComponent* AudioComponent = nullptr;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* PlayerCamera = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponent = nullptr;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Change la position du CasaPlayer au coordonnees donnees par le FVector2D (Z par defaut 50)
	UFUNCTION()
	void MoveTo(FVector2D TargetPosition);

	// Sets default values for this pawn's properties
	ACasaPlayer();
	//Objet tenu par le Joueur
	AItem* HeldItem;

};
