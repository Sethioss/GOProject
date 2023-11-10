// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CasaPlayer.generated.h"

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


public:
	// Sets default values for this pawn's properties
	ACasaPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	UFUNCTION()
	void Move_XAxis(float AxisValue);
	UFUNCTION()
	void Move_YAxis(float AxisValue);
	UFUNCTION()
	void Test();

	//Change la position du CasaPlayer au coordonnees donnees par le FVector2D (Z par defaut 0)
	UFUNCTION()
	void MoveTo(FVector2D TargetPosition);

	// Input variables
	UPROPERTY()
	FVector CurrentVelocity;

};
