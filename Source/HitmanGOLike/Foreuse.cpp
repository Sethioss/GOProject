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
	SetIsHeld();
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
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
			CurrentNode = NewNode;
		}
		if(NodePos != FVector(NULL,NULL,NULL))
		{
			//On appelle la fonction en donnant un FVector
			SetActorLocation(FVector(NodePos.X, NodePos.Y,50));
			CurrentNode = NewNode;
		}
	}
}