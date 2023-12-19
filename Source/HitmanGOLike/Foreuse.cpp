#include "Foreuse.h"
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
	Remove();
}

void AForeuse::Remove()
{
	Usable = false;
	//TEMPORARY - SAFELY REMOVE FROM BOARD
	SetActorLocation(FVector(10000, 10000, 10000));
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
	CurrentNode->IsObstacle = false;
	CurrentNode->HasObjectOnIt = false;
	CurrentNode = nullptr;
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
			SetActorLocation(FVector(NewNode->GetActorLocation().X, NewNode->GetActorLocation().Y, NewNode->GetActorLocation().Z));
			CurrentNode->IsObstacle = false;
			CurrentNode->HasObjectOnIt = false;
			CurrentNode = NewNode;
			CurrentNode->HasObjectOnIt = true;
			CurrentNode->IsObstacle = true;
		}
		if(NodePos != FVector(NULL,NULL,NULL))
		{
			//On appelle la fonction en donnant un FVector
			SetActorLocation(FVector(NodePos.X, NodePos.Y, NewNode->GetActorLocation().Z));
			CurrentNode->IsObstacle = false;
			CurrentNode->HasObjectOnIt = false;
			CurrentNode = NewNode;
			CurrentNode->HasObjectOnIt = true;
			CurrentNode->IsObstacle = true;
		}
	}
}