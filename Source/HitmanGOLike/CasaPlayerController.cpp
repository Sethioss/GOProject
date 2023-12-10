// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Foreuse.h"
#include "Otage.h"
#include "PathActor.h"

// Définissez une fonction pour gérer le clic de souris
FVector	WorldPosition, WorldDirection;

void ACasaPlayerController::OnMouseClick()
{
	// Récupérez la position de la souris
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	// Maintenant, vous avez les coordonnées X et Y de la souris dans MousePosition
	float MouseX = MousePosition.X;
	float MouseY = MousePosition.Y;

	FHitResult HitResult;

	FVector2D MousePos(MouseX, MouseY);

	// Faites quelque chose avec les coordonnées (par exemple, tracez-les dans la console)
	UE_LOG(LogTemp, Warning, TEXT("Coordonnées de la souris - X: %f, Y: %f"), MouseX, MouseY);

	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());
	if (PlayerFinal)
	{

		ACasaPlayerController* MyController = Cast<ACasaPlayerController>(GetWorld()->GetFirstPlayerController());

		if (MyController) {
			UGameplayStatics::DeprojectScreenToWorld(MyController, MousePos, WorldPosition, WorldDirection);
			GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + WorldDirection * 10000,
				ECollisionChannel::ECC_GameTraceChannel1);
		}

		//DrawDebugLine(GetWorld(), WorldPosition, WorldPosition + WorldDirection * 10000, FColor::Red, true, 50.f);
		//DrawDebugLine(GetWorld(), WorldPosition, HitResult.ImpactPoint, FColor::Green, true, 50.f);
		//DrawDebugSphere(GetWorld(), WorldPosition, 25.f, 16, FColor::Red, true, 50.f);

		if (HitResult.bBlockingHit)
		{
			
			APathActor* Path = Cast<APathActor>(HitResult.GetActor());

			if (Path)
			{
				if (Path->GetIsNode())
				{
					

					if(PlayerFinal->HeldItem == nullptr){
						if (Path->IsPlayerOnNeighbouringNode())
						{
							FVector2D ActorLocation(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y);
							PlayerFinal->MoveTo(ActorLocation);
							UE_LOG(LogTemp, Warning, TEXT("It's a path with a node"));
						}
					}
					else {
						AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
						if (Foreuse) {
							UE_LOG(LogTemp, Warning, TEXT("Target X %f Y %f player X %f Y %f"), Path->GetActorLocation().X, Path->GetActorLocation().Y, PlayerFinal->GetActorLocation().X,PlayerFinal->GetActorLocation().Y);
							if (Path->GetActorLocation().X != PlayerFinal->GetActorLocation().X || Path->GetActorLocation().Y != PlayerFinal->GetActorLocation().Y)
							{
								APathActor* NewNodeForPlayer = Path->IsForeuseOnNeighbourinNode();
								if (NewNodeForPlayer) {
									UE_LOG(LogTemp, Warning, TEXT("Move"));
									PlayerFinal->MoveTo(FVector2D(NewNodeForPlayer->GetActorLocation().X, NewNodeForPlayer->GetActorLocation().Y));
									Foreuse->SetForeuseLocation(Path, FVector(NULL, NULL, NULL));
								}
								else {
									if ((Path->GetActorLocation().X == Foreuse->GetActorLocation().X && Foreuse->GetActorLocation().X == PlayerFinal->GetActorLocation().X) || (Path->GetActorLocation().Y == Foreuse->GetActorLocation().Y && Foreuse->GetActorLocation().Y == PlayerFinal->GetActorLocation().Y))
									{
										if (Path->IsPlayerOnNeighbouringNode())
										{
											FVector2D ActorLocation(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y);
											Foreuse->SetForeuseLocation(nullptr, PlayerFinal->GetActorLocation());
											PlayerFinal->MoveTo(ActorLocation);

										}
									}
								}
							}
						}
						else {
							AOtage* Otage = Cast<AOtage>(PlayerFinal->HeldItem);
							if(Otage)
							{
								if (Otage->Placable)
								{
									if (Otage->PlacingArea.IsInside(Path->GetActorLocation()))
									{
										Otage->SetOtageLocation(Path);
										PlayerFinal->HeldItem = nullptr;
									}
								}
							}
						}
					}
				}
			}
			AItem* Item = Cast<AItem>(HitResult.GetActor());
			if (Item) {
				UE_LOG(LogTemp, Warning, TEXT("It's an Item"));
				PlayerFinal->HeldItem = Item;
				Item->SetIsHeld();
				UE_LOG(LogTemp, Warning, TEXT("Item equiped"));
			}
		}
	}

}

// Associez cette fonction à un événement de clic de souris
ACasaPlayerController::ACasaPlayerController()
{
	// Associez la fonction OnMouseClick() à l'événement de clic de souris
	bShowMouseCursor = true; // Assurez-vous que le curseur de la souris est visible
	DefaultMouseCursor = EMouseCursor::Crosshairs; // Définissez le curseur par défaut
}

void ACasaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Associez le clic gauche de la souris à la fonction OnMouseClick()
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ACasaPlayerController::OnMouseClick);
	// Associez la touche "F" à la fonction OnPressF()
	InputComponent->BindAction("PressF", IE_Pressed, this, &ACasaPlayerController::OnPressF);
}

void ACasaPlayerController::OnPressF()
{
	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());
	// Récupérez l'objet actuellement tenu par le joueur (si disponible)
	AItem* HeldItem = PlayerFinal->HeldItem;

	// Vérifiez si l'objet est valide
	if (HeldItem)
	{
		// Appellez la fonction SetIsHeld() de l'objet
		HeldItem->SetIsHeld();
		PlayerFinal->HeldItem = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Item unequiped"));
	}
}