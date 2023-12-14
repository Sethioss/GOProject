// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITMANGOLIKE_API UMoverComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoverComponent();

	UPROPERTY(EditAnywhere, Category = "MyCurve")
	UCurveFloat* MovementCurve = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector StartPosition;
	FVector DestinationPosition;
	float Clock = 0;

	bool ClockStarted = false;

public:	

	void Setup(FVector DestinationPos);
	bool MoveTo(float TimeItTakes);

		
};
