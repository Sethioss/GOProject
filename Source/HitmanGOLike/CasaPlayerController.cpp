// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PathActor.h"
#include "Foreuse.h"
#include "Otage.h"
#include "Wall.h"

// D�finissez une fonction pour g�rer le clic de souris
FVector	WorldPosition, WorldDirection;

void ACasaPlayerController::OnMouseClick()
{
	// R�cup�ration des coordonn�es �cran de la souris
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
			//LineCast avec conversion des coordonn�es �cran en coordonn�es world 
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
								//Path n'est pas le Node o� le joueur se trouve
								APathActor* NewNodeForPlayer = Path->IsForeuseOnNeighbourinNode();

								if (NewNodeForPlayer) 
								{
									//La Foreuse est sur un Node voisin de Path (Deplacement vers l'avant de la foreuse)
									PlayerFinal->MoveTo(FVector2D(NewNodeForPlayer->GetActorLocation().X, NewNodeForPlayer->GetActorLocation().Y));
									Foreuse->SetForeuseLocation(Path, FVector(NULL, NULL, NULL));
								}
								else 
								{
									//La Foreuse n'est pas voisine de Path (Deplacement vers l'arri�re ou d�placement non valide ?)
									if ((Path->GetActorLocation().X == Foreuse->GetActorLocation().X && Foreuse->GetActorLocation().X == PlayerFinal->GetActorLocation().X) || (Path->GetActorLocation().Y == Foreuse->GetActorLocation().Y && Foreuse->GetActorLocation().Y == PlayerFinal->GetActorLocation().Y))
									{
										//Le Joueur, La Foreuse et Path sont bien align�s
										if (Path->IsPlayerOnNeighbouringNode())
										{
											//Le Joueur est voisin de Path (Il s'agit d'un d�placement valide)
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
									//L'Otage n'a pas encore �t� utilis�
									if (Otage->PlacingArea.IsInsideXY(Path->GetActorLocation()))
									{
										//Path est dans la PlacingArea de l'Otage 
										APathActor* OtageNode = Otage->GetCurrentNode();
										OtageNode->IsPlayerOnNeighbouringNode();
										PlayerFinal->SetActorLocation(Otage->GetActorLocation());
										Otage->SetOtageLocation(Path);
										PlayerFinal->HeldItem = nullptr;
										//Otage->ItemEffect();
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
				if (Item->GetCurrentNode()->IsPlayerOnNeighbouringNodeWithoutOwnershipTransfer())
				{
					//L'Objet est un Item on l'�quipe au Joueur
					PlayerFinal->HeldItem = Item;
					Item->SetIsHeld();
				}
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
							//La Foreuse, Le Wall et le Joueur sont bien align�s
							APathActor* NewNodeForPlayer = Wall->CurrentNode->IsForeuseOnNeighbourinNode();
							if (NewNodeForPlayer) 
							{
								//La Foreuse est bien � c�t� du Wall
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

// Associez cette fonction � un �v�nement de clic de souris
ACasaPlayerController::ACasaPlayerController()
{
	// Associez la fonction OnMouseClick() � l'�v�nement de clic de souris
	bShowMouseCursor = true; // Assurez-vous que le curseur de la souris est visible
	DefaultMouseCursor = EMouseCursor::Crosshairs; // D�finissez le curseur par d�faut
}

void ACasaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Associez le clic gauche de la souris � la fonction OnMouseClick()
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ACasaPlayerController::OnMouseClick);
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