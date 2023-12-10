// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation du component staticmesh
	StaticMeshComponent =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visuel"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	StaticMeshComponent->SetStaticMesh(meshFinder.Object);

	// creation du component audio
	AudioComponent =
		CreateDefaultSubobject<UAudioComponent>(TEXT("Son"));
}

// Sets default values
AItem::AItem(APathActor* CurrentNode)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation du component staticmesh
	StaticMeshComponent =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visuel"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cube.Cube")); //static permet d'executer la fonction qu'une fois
	StaticMeshComponent->SetStaticMesh(meshFinder.Object);

	// creation du component audio
	AudioComponent =
		CreateDefaultSubobject<UAudioComponent>(TEXT("Son"));


}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation()+ FVector(0, 0, 1), GetActorLocation()+FVector(0,0,-1)*1000,
		ECollisionChannel::ECC_GameTraceChannel1);
	if (HitResult.bBlockingHit)
	{
		APathActor* Node = Cast<APathActor>(HitResult.GetActor());
		if(Node)
		{
			CurrentNode = Node;
			SetActorLocation(Node->GetActorLocation());
		}
	}
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
APathActor* AItem::IsForwardNodeValid()
{
	FHitResult ForwardObject;
	//Looking for Northern Object
	GetWorld()->LineTraceSingleByChannel(ForwardObject, CurrentNode->GetActorLocation() + FVector(0, 0, 51), CurrentNode->GetActorLocation() + (GetActorForwardVector() * 1000),
		ECollisionChannel::ECC_GameTraceChannel1);

	APathActor* Node = Cast<APathActor>(ForwardObject.GetActor());

	if (Node) return Node;

	
	// ABreakableWall* Wall = Cast<ABreakableWall>(ForwardObject.GetActor());
	// if (Wall) return Wall;
	

	
	return nullptr;
}
*/


APathActor* AItem::IsBackwardNodeValid()
{
	return nullptr;
}

void AItem::ItemEffect()
{
}

void AItem::SetIsHeld() 
{
	IsHeld = !IsHeld; 
}