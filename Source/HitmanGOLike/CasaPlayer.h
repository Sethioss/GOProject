// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "SecurityGuardEnemy.h"
#include "GameManager.h"
#include "MoverComponent.h"
#include "Item.h"
#include "Foreuse.h"
#include "CasaPlayer.generated.h"

UCLASS()
class HITMANGOLIKE_API ACasaPlayer : public APawn, public IBoardElement
{
	GENERATED_BODY()


protected:
	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditAnywhere)
	float Speed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponent = nullptr;

	FVector2D TargetedPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMoverComponent* MoverComponent = nullptr;


public:
	// Sets default values for this pawn's properties
	ACasaPlayer();

	APathActor* SnapToGrid(FVector offset);

	APathActor* CurrentNode;
	UPROPERTY(VisibleAnywhere)
	bool TurnFinished = false;

	bool RegisteredToManager = false;
	void RegisterToManager();

	UPROPERTY(EditAnywhere)
	class UCameraComponent* PlayerCamera = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Change la position du CasaPlayer au coordonnees donnees par le FVector2D (Z par defaut 0)
	UFUNCTION()
	void MoveTo(FVector2D TargetPosition);

	void MoveTo(APathActor* TargetPosition);

	void InitiateMovement(APathActor* Target);
	void InitiateMovementWithDrill(APathActor* Target, APathActor* ForeuseTarget);

	bool Finished = false;

	// Input variables
	UPROPERTY()
	FVector CurrentVelocity;

	//Objet tenu par le Joueur
	AItem* HeldItem;

};
