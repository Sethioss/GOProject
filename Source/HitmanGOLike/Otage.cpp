// Fill out your copyright notice in the Description page of Project Settings.

#include "Otage.h"
#include "EnemyActor.h"

void AOtage::BeginPlay() 
{
	Super::BeginPlay();
	FVector PlacingBoxExtent = FVector(((GetActorScale().X * 50) * 6) + 1, ((GetActorScale().X * 50) * 6) + 1, 100);
	PlacingArea = FBox::BuildAABB(GetActorLocation(), PlacingBoxExtent);
	UE_LOG(LogTemp, Warning, TEXT("PlaceArea minX : %f Path X: %f PlaceArea maxX : %f , PlaceArea minY : %f Path Y: %f PlaceArea maxY : %f"), (GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10)).X, 100.0f, (GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 3) + 1, 100)).X, (GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10)).Y, 100.0f, (GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 3) + 1, 100)).Y);
	
}

void AOtage::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	if(IsHeld)
		DrawDebugBox(GetWorld(), GetActorLocation(), FVector(((GetActorScale().X * 50) * 6) + 1, ((GetActorScale().X * 50) * 6) + 1, 100), FColor::Green);
}

void AOtage::SetOtageLocation(APathActor* Target)
{
	if (IsHeld && Placable)
	{	
		//L'Otage peut être déplacé
		SetActorLocation(FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y,50));
		CurrentNode = Target;
		Placable = false;
		SetIsHeld();
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
		ItemEffect();
	}
}

//Cherche les Ennemis a range de cri, et les alertes en leur donnant sa position.
//void AOtage::ItemEffect() {
//	
//
//	std::vector<AEnemyActor*> EnemiesArray;
//	FHitResult HitResult;
//
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0,0,25), GetActorLocation()+FVector(50,0,25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(50, 50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(0, 50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 0, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, -50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(0, -50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, 25), GetActorLocation() + FVector(-50, 50, 25),
//		ECollisionChannel::ECC_GameTraceChannel1);
//	if (HitResult.bBlockingHit)
//	{
//		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
//		if (Enemy)
//		{
//			EnemiesArray.push_back(Enemy);
//		}
//	}
//
//	//Après avoir recupérer tous les ennemies à range de cri, les alertes
//	for (AEnemyActor temp : EnemiesArray)
//	{
//		temp.Alert(GetActorLocation());
//	}
//
//}