// Fill out your copyright notice in the Description page of Project Settings.


#include "Foreuse.h"

AForeuse::AForeuse(APathActor* CurrentNode) : AItem(CurrentNode)
{
	
}
AForeuse::AForeuse(): AItem() {

}

void AForeuse::ItemEffect()
{
	APathActor* ForwardNode = IsForwardNodeValid();
	if (ForwardNode!= nullptr && IsHeld)
	{
		SetActorLocation(ForwardNode->GetActorLocation());
	}

}