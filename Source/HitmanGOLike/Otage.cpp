// Fill out your copyright notice in the Description page of Project Settings.

#include "Otage.h"
#include "EnemyActor.h"

void AOtage::BeginPlay() 
{
	Super::BeginPlay();
	FVector PlacingBoxExtent = FVector(((GetActorScale().X * 50) * 8) + 1, ((GetActorScale().X * 50) * 8) + 1, 100);
	PlacingArea = FBox::BuildAABB(GetActorLocation(), PlacingBoxExtent);
	UE_LOG(LogTemp, Warning, TEXT("PlaceArea minX : %f Path X: %f PlaceArea maxX : %f , PlaceArea minY : %f Path Y: %f PlaceArea maxY : %f"), (GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10)).X, 100.0f, (GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 3) + 1, 100)).X, (GetActorLocation() + FVector(-(((GetActorScale().X * 50) * 3) + 1), -(((GetActorScale().X * 50) * 3) + 1), -10)).Y, 100.0f, (GetActorLocation() + FVector(((GetActorScale().X * 50) * 3) + 1, ((GetActorScale().X * 50) * 3) + 1, 100)).Y);
	
}

void AOtage::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	if(IsHeld)
		DrawDebugBox(GetWorld(), GetActorLocation(), FVector(((GetActorScale().X * 50) * 8) + 1, ((GetActorScale().X * 50) * 8) + 1, 100), FColor::Green);
}

void AOtage::SetOtageLocation(APathActor* Target)
{
	if (IsHeld && Placable)
	{	
		//L'Otage peut être déplacé
		SetActorLocation(FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y,50));
		CurrentNode->HasObjectOnIt = false;
		CurrentNode = Target;
		CurrentNode->HasObjectOnIt = true;
		Placable = false;
		SetIsHeld();
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
		ItemEffect();
	}
}

//Cherche les Ennemis a range de cri, et les alertes en leur donnant sa position.
void AOtage::ItemEffect() {

	TArray<AEnemyActor*> EnemiesArray;
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0,0, -15), GetActorLocation()+FVector(200,0, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(200, 200, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(0, 200, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(-200, 100, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(-200, 0, -15),
		ECollisionChannel::ECC_Visibility);
	FString Vec = FVector(GetActorLocation() + FVector(0, 0, -15)).ToString();
	DrawDebugLine(GetWorld(), GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(0, -200, -15), FColor::Red, true, 99999.f);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(-200, -200, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(0, -200, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + FVector(0, 0, -15), GetActorLocation() + FVector(-200, 200, -15),
		ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AEnemyActor* Enemy = Cast<AEnemyActor>(HitResult.GetActor());
		if (Enemy)
		{
			EnemiesArray.Add(Enemy);
		}
	}

	//Après avoir recupérer tous les ennemies à range de cri, les alertes
	for (int i = 0; i < EnemiesArray.Num(); ++i)
	{
		EnemiesArray[i]->Alert(this);
	}

}