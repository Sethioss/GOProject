// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyActor.h"
#include "Otage.h"

void AOtage::BeginPlay() {
	Super::BeginPlay();
	FVector PlacingBoxMin = GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10);
	FVector PlacingBoxMax = GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 3) + 1, 100);
	PlacingArea = FBox(PlacingBoxMin, PlacingBoxMax);
}

void AOtage::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if(IsHeld)
		DrawDebugBox(GetWorld(), GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10), GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 2) + 1, 100), FColor::Green);
}

void AOtage::SetOtageLocation(APathActor* Target)
{
	if (IsHeld && Placable)
	{
		SetActorLocation(FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y,50));
		CurrentNode = Target;
		Placable = false;
		SetIsHeld();
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);

		//ItemEffect();
	}
}

void AOtage::ItemEffect() {
	//Cherche les Ennemis a range de cri, et les alertes en leur donnant sa position.

	std::vector<AEnemyActor*> EnemiesArray;
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0,0,25), GetActorLocation()+FVector(50,0,25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(50, 50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(0, 50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 0, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, -50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(0, -50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 50, 25),
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.push_back(Enemy);
		}
	}

	for (AEnemyActor temp : EnemiesArray)
	{
		temp.Alert(GetActorLocation());
	}

}