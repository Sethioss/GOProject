#include "Foreuse.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Foreuse.h"
#include "CasaPlayer.h"

AForeuse::AForeuse(APathActor* CurrentNode) : AItem(CurrentNode)
{
	
}
AForeuse::AForeuse(): AItem() 
{

}

void AForeuse::ItemEffect(AWall* Wall)
{
	Wall->Break();
	Usable = false;
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
}

void AForeuse::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Making this node an obstacle"));
	CurrentNode->IsObstacle = true;
}

void AForeuse::SetForeuseLocation(APathActor* NewNode, FVector NodePos) 
{
	if (IsHeld)
	{
		//La Foreuse est tenue
		if(NewNode)
		{
			//On appelle la fonction en donnant un Node
			SetActorLocation(FVector(NewNode->GetActorLocation().X, NewNode->GetActorLocation().Y,50));
			CurrentNode->IsObstacle = false;
			CurrentNode = NewNode;
			CurrentNode->IsObstacle = true;
		}
		if(NodePos != FVector(NULL,NULL,NULL))
		{
			//On appelle la fonction en donnant un FVector
			SetActorLocation(FVector(NodePos.X, NodePos.Y,50));
			CurrentNode->IsObstacle = false;
			CurrentNode = NewNode;
			CurrentNode->IsObstacle = true;
		}
	}
}