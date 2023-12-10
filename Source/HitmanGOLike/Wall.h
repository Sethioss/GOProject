// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "PathActor.h"
#include "Wall.generated.h"

UCLASS()
class HITMANGOLIKE_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	bool IsBroken = false;

	void Break();
	APathActor* CurrentNode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BluePrintReadOnly, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY()
	class UAudioComponent* AudioComponent = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
