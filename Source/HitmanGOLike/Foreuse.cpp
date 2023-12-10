// Fill out your copyright notice in the Description page of Project Settings.


#include "Foreuse.h"
#include "CasaPlayer.h"

AForeuse::AForeuse(APathActor* CurrentNode) : AItem(CurrentNode)
{
	
}
AForeuse::AForeuse(): AItem() {

}

void AForeuse::ItemEffect()
{
	

}

void AForeuse::SetForeuseLocation(APathActor* NewNode, FVector NodePos) {

	if (IsHeld)
	{
		if(NewNode)
		{
			SetActorLocation(NewNode->GetActorLocation());
			CurrentNode = NewNode;
		}
		if(NodePos != FVector(NULL,NULL,NULL)){
			SetActorLocation(NodePos);
			CurrentNode = NewNode;
		}
	}
}