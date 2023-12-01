// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "CasaPlayer.h"
#include "GameManager.h"

// Sets default values for this component's properties
UGameManager::UGameManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<AActor*> EnemiesToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActor::StaticClass(), EnemiesToFind);

	for (AActor* enemy : EnemiesToFind)
	{
		AEnemyActor* MyActor = Cast<AEnemyActor>(enemy);

		if (MyActor != nullptr) {
			// Do what ever you need with your MyActor
			Enemies.Add(MyActor);
		}
	}

	AActor* act;
	act = UGameplayStatics::GetActorOfClass(GetWorld(), ACasaPlayer::StaticClass());

	ACasaPlayer* pl = Cast<ACasaPlayer>(act);

	if (pl != nullptr) {
		Player = pl;
	}
	
}


// Called every frame
void UGameManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (Player->TurnFinished)
	{
		for (AEnemyActor* Enemy : Enemies)
		{
			Enemy->Update();
		}
	}
	Player->TurnFinished = false;
}

