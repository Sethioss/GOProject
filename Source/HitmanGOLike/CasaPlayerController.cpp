// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Foreuse.h"
#include "Otage.h"
#include "PathActor.h"
#include "Wall.h"

// Définissez une fonction pour gérer le clic de souris
FVector	WorldPosition, WorldDirection;

void ACasaPlayerController::OnMouseClick()
{
	// Récupération des coordonnées écran de la souris
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	float MouseX = MousePosition.X;
	float MouseY = MousePosition.Y;
	FHitResult HitResult;
	FVector2D MousePos(MouseX, MouseY);
	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());

	if (PlayerFinal)
	{

		ACasaPlayerController* MyController = Cast<ACasaPlayerController>(GetWorld()->GetFirstPlayerController());

		if (MyController) 
		{
			//LineCast avec conversion des coordonnées écran en coordonnées world 
			UGameplayStatics::DeprojectScreenToWorld(MyController, MousePos, WorldPosition, WorldDirection);
			GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + WorldDirection * 10000,
				ECollisionChannel::ECC_GameTraceChannel1);
		}

		//Si le LineCast touche un objet
		if (HitResult.bBlockingHit)
		{
			
			APathActor* Path = Cast<APathActor>(HitResult.GetActor());

			if (Path)
			{
				//L'objet est un chemin
				if (Path->GetIsNode())
				{
					//Le chemin est un Node
					if(PlayerFinal->HeldItem == nullptr)
					{
						//Le Joueur ne tient pas d'objet
						if (Path->IsPlayerOnNeighbouringNode())
						{
							//Le Joueur est sur un Node voisin de Path
							FVector2D ActorLocation(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y);
							PlayerFinal->MoveTo(ActorLocation);
						}
					}
					else 
					{
						//Le Joeur tient un item
						AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
						if (Foreuse&&Foreuse->Usable) 
						{
							//C'est une Foreuse
							if (Path->GetActorLocation().X != PlayerFinal->GetActorLocation().X || Path->GetActorLocation().Y != PlayerFinal->GetActorLocation().Y)
							{
								//Path n'est pas le Node où le joueur se trouve
								APathActor* NewNodeForPlayer = Path->IsForeuseOnNeighbourinNode();

								if (NewNodeForPlayer) 
								{
									//La Foreuse est sur un Node voisin de Path (Deplacement vers l'avant de la foreuse)
									PlayerFinal->MoveTo(FVector2D(NewNodeForPlayer->GetActorLocation().X, NewNodeForPlayer->GetActorLocation().Y));
									Foreuse->SetForeuseLocation(Path, FVector(NULL, NULL, NULL));
								}
								else 
								{
									//La Foreuse n'est pas voisine de Path (Deplacement vers l'arrière ou déplacement non valide ?)
									if ((Path->GetActorLocation().X == Foreuse->GetActorLocation().X && Foreuse->GetActorLocation().X == PlayerFinal->GetActorLocation().X) || (Path->GetActorLocation().Y == Foreuse->GetActorLocation().Y && Foreuse->GetActorLocation().Y == PlayerFinal->GetActorLocation().Y))
									{
										//Le Joueur, La Foreuse et Path sont bien alignés
										if (Path->IsPlayerOnNeighbouringNode())
										{
											//Le Joueur est voisin de Path (Il s'agit d'un déplacement valide)
											FVector2D ActorLocation(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y);
											Foreuse->SetForeuseLocation(nullptr, PlayerFinal->GetActorLocation());
											PlayerFinal->MoveTo(ActorLocation);
										}
									}
								}
							}
						}
						else 
						{
							AOtage* Otage = Cast<AOtage>(PlayerFinal->HeldItem);
							if(Otage)
							{
								//L'item est un Otage
								if (Otage->Placable)
								{
									//L'Otage n'a pas encore été utilisé
									if (Otage->PlacingArea.IsInsideXY(Path->GetActorLocation()))
									{
										//Path est dans la PlacingArea de l'Otage 
										APathActor* OtageNode = Otage->GetCurrentNode();
										OtageNode->IsPlayerOnNeighbouringNode();
										PlayerFinal->SetActorLocation(Otage->GetActorLocation());
										Otage->SetOtageLocation(Path);
										PlayerFinal->HeldItem = nullptr;
										Otage->ItemEffect();
									}
								}
							}
						}
					}
				}
			}
			AItem* Item = Cast<AItem>(HitResult.GetActor());
			if (Item) 
			{
				//L'Objet est un Item on l'équipe au Joueur
				PlayerFinal->HeldItem = Item;
				Item->SetIsHeld();
			}
			AWall* Wall = Cast<AWall>(HitResult.GetActor());
			if (Wall)
			{
				//C'est un Wall
				if (PlayerFinal->HeldItem)
				{
					//Le Joueur a un Item
					AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
					if (Foreuse&&Foreuse->Usable)
					{
						//C'est une Foreuse et elle est utilisable
						if ((Wall->GetActorLocation().X == Foreuse->GetActorLocation().X && Foreuse->GetActorLocation().X == PlayerFinal->GetActorLocation().X) || (Wall->GetActorLocation().Y == Foreuse->GetActorLocation().Y && Foreuse->GetActorLocation().Y == PlayerFinal->GetActorLocation().Y))
						{
							//La Foreuse, Le Wall et le Joueur sont bien alignés
							APathActor* NewNodeForPlayer = Wall->CurrentNode->IsForeuseOnNeighbourinNode();
							if (NewNodeForPlayer) 
							{
								//La Foreuse est bien à côté du Wall
								Foreuse->ItemEffect(Wall);
								PlayerFinal->MoveTo(FVector2D(NewNodeForPlayer->GetActorLocation().X, NewNodeForPlayer->GetActorLocation().Y));
								Foreuse->SetForeuseLocation(Wall->CurrentNode, FVector(NULL, NULL, NULL));
								PlayerFinal->HeldItem = nullptr;
							}
						}
					}
				}
			}
		}
	}

}


ACasaPlayerController::ACasaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ACasaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ACasaPlayerController::OnMouseClick);
	InputComponent->BindAction("PressF", IE_Pressed, this, &ACasaPlayerController::OnPressF);
}

//Désequipe l'objet du Joueur
void ACasaPlayerController::OnPressF()
{
	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());
	AItem* HeldItem = PlayerFinal->HeldItem;

	if (HeldItem)
	{
		// Le Joueur possède un Objet
		HeldItem->SetIsHeld();
		PlayerFinal->HeldItem = nullptr;
	}
}