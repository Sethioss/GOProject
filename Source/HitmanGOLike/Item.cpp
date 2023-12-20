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


}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	CurrentNode = SnapToGrid();

	//FHitResult HitResult;
	//GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation()+ FVector(0, 0, 1), GetActorLocation()+FVector(0,0,-1)*1000,
	//	ECollisionChannel::ECC_GameTraceChannel1);
	//if (HitResult.bBlockingHit)
	//{
	//	//Si l'Item n'est pas bien positionner lors du LevelBuilding il sera automatiquement repositionné au centre de la Node
	//	APathActor* Node = Cast<APathActor>(HitResult.GetActor());
	//	if(Node)
	//	{
	//		CurrentNode = Node;
	//		SetActorLocation(FVector(Node->GetActorLocation().X, Node->GetActorLocation().Y,50));
	//	}
	//}
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APathActor* AItem::IsBackwardNodeValid()
{
	return nullptr;
}

void AItem::SetIsHeld() 
{
	IsHeld = !IsHeld; 
}

APathActor* AItem::SnapToGrid(FVector offset)
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), -(GetActorUpVector() * 1000), ECollisionChannel::ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - (GetActorUpVector() * 1000), FColor::Green, true, 50.f);

	if (HitResult.bBlockingHit)
	{

		APathActor* Path = Cast<APathActor>(HitResult.GetActor());

		if (Path)
		{
			CurrentNode = Path;

			FBox ActorBounds = GetComponentsBoundingBox();

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z));
			Path->HasObjectOnIt = true;
		}
	}

	return CurrentNode;
}