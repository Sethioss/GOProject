// Fill out your copyright notice in the Description page of Project Settings.


#include "Otage.h"

void AOtage::BeginPlay() {
	Super::BeginPlay();
	PlacingArea = FBox(GetActorLocation()+ FVector(-75, -125, -100) , GetActorLocation()+FVector(75, 125, 200));
	EffectArea = FBox(GetActorLocation() + FVector(0, 0, -10), GetActorLocation() * 3);
	//+(CurrentNode->PlaneMesh->GetStaticMesh()->GetBoundingBox().GetSize()*2)
}

void AOtage::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if(IsHeld)
		DrawDebugBox(GetWorld(), GetActorLocation() + FVector(-75, -125, -100), GetActorLocation() + FVector(75, 125, 200), FColor::Green);
}

void AOtage::SetOtageLocation(APathActor* Target)
{
	if (IsHeld && Placable)
	{
		SetActorLocation(Target->GetActorLocation());
		CurrentNode = Target;
		Placable = false;
		SetIsHeld();
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
		//ItemEffect();
	}
}

void AOtage::ItemEffect() {
	
}