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
	ACasaPlayer* PlayerFinal = UGameManager::GetInstance()->Player;

	if (PlayerFinal)
	{
		if (!PlayerFinal->TurnFinished)
		{
			ACasaPlayerController* MyController = Cast<ACasaPlayerController>(GetWorld()->GetFirstPlayerController());

			if (MyController)
			{
				//LineCast avec conversion des coordonn�es �cran en coordonn�es world 
				UGameplayStatics::DeprojectScreenToWorld(MyController, MousePosition, WorldPosition, WorldDirection);

				DrawDebugLine(GetWorld(), WorldPosition, WorldDirection * 10000, FColor::White, true);
				GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition,  WorldPosition + WorldDirection * 10000,
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
						if (!UGameManager::GetInstance()->CheckIfWall(Path, UGameManager::GetInstance()->GetPlayerNode(), true)
							|| !Path->HasObjectOnIt)
						{
							//Le chemin est un Node
							if (PlayerFinal->HeldItem == nullptr)
							{
								APathActor* Target = Path->IsPlayerOnNeighbouringNode();
								if (Target && !Target->HasObjectOnIt)
								{
									PlayerFinal->InitiateMovement(Target);
								}
							}
							else
							{
								//Le Joeur tient un item
								AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
								if (Foreuse && Foreuse->Usable)
								{
									//C'est une Foreuse
									if (Path->GetActorLocation().X != PlayerFinal->GetActorLocation().X || Path->GetActorLocation().Y != PlayerFinal->GetActorLocation().Y)
									{
										//Path n'est pas le Node o� le joueur se trouve
										APathActor* NewNodeForPlayer = Path->IsPlayerOnNeighbouringNode();

										if (!NewNodeForPlayer)
										{
											if (!UGameManager::GetInstance()->CheckIfWall(Path, Foreuse->CurrentNode, true))
											{
												//Le Joueur, La Foreuse et Path sont bien align�s
												APathActor* Target = Path->IsForeuseOnNeighbouringNode();
												if (Target)
												{
													//Recuperer la direction player->foreuse
													APathActor* PlayerNode = UGameManager::GetInstance()->Player->CurrentNode;

													for (int i = 0; i < PlayerNode->ConnectorInfo.Num(); ++i)
													{
														if (PlayerNode->ConnectorInfo[i].DestinationNode == Foreuse->CurrentNode)
														{
															//Recuperer la direction foreuse->path
															for (int j = 0; j < Foreuse->CurrentNode->ConnectorInfo.Num(); ++j)
															{
																if (Foreuse->CurrentNode->ConnectorInfo[j].DestinationNode == Target)
																{
																	UE_LOG(LogTemp, Error, TEXT("Found direction between foreuse and path"));

																	//Si c'est la même direction que player->foreuse
																	if (Foreuse->CurrentNode->ConnectorInfo[j].Direction == PlayerNode->ConnectorInfo[i].Direction)
																	{
																		PlayerFinal->InitiateMovementWithDrill(Foreuse->CurrentNode, Target);
																	}
																}
															}
														}
													}
												}
											}

										}
										else
										{
											//La Foreuse n'est pas voisine de Path (Deplacement vers l'arri�re ou d�placement non valide ?)
											APathActor* Targ = Path->IsForeuseOnNeighbouringNode();
											if (!Targ)
											{
												//Le Joueur, La Foreuse et Path sont bien align�s
												APathActor* Target = Path->IsPlayerOnNeighbouringNode();
												if (Target)
												{
													//Recuperer la direction foreuse->player
													APathActor* PlayerNode = UGameManager::GetInstance()->Player->CurrentNode;

													for (int i = 0; i < Foreuse->CurrentNode->ConnectorInfo.Num(); ++i)
													{
														if (Foreuse->CurrentNode->ConnectorInfo[i].DestinationNode == PlayerNode)
														{
															//UE_LOG(LogTemp, Error, TEXT("Found direction between foreuse and player"));

															//Recuperer la direction player->path
															for (int j = 0; j < PlayerNode->ConnectorInfo.Num(); ++j)
															{
																if (PlayerNode->ConnectorInfo[j].DestinationNode == Target)
																{
																	//UE_LOG(LogTemp, Error, TEXT("Found direction between player and path"));

																	//Si c'est la même direction que foreuse->player
																	if (PlayerNode->ConnectorInfo[j].Direction == Foreuse->CurrentNode->ConnectorInfo[i].Direction)
																	{
																		PlayerFinal->InitiateMovementWithDrill(Target, PlayerNode);
																	}
																}
															}
														}
													}


													//Foreuse->SetForeuseLocation(UGameManager::GetInstance()->GetPlayerNode(), PlayerFinal->GetActorLocation());
													//PlayerFinal->InitiateMovement(Target);
												}
											}
										}
									}
								}
								else
								{
									AOtage* Otage = Cast<AOtage>(PlayerFinal->HeldItem);
									if (Otage)
									{
										//L'item est un Otage
										if (Otage->Placable)
										{
											//L'Otage n'a pas encore �t� utilis�
											if (Otage->PlacingArea.IsInsideXY(Path->GetActorLocation()))
											{
												//Path est dans la PlacingArea de l'Otage 
												APathActor* OtageNode = Otage->GetCurrentNode();
												APathActor* Target = OtageNode->IsPlayerOnNeighbouringNode();
												if (Target)
												{
													PlayerFinal->InitiateMovement(Target);
												}
												PlayerFinal->SetActorLocation(Otage->GetActorLocation());
												UGameManager::GetInstance()->PlaySound("SND_OtageHelp");
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
				}
				AItem* Item = Cast<AItem>(HitResult.GetActor());
				if (Item)
				{
					PlayerFinal = Cast<ACasaPlayer>(GetPawn());
					AItem* HeldItem = PlayerFinal->HeldItem;

					if (HeldItem)
					{
						// Le Joueur possède un Objet
						UE_LOG(LogTemp, Error, TEXT("Unequipping foreuse!"));
						HeldItem->SetIsHeld();
						PlayerFinal->HeldItem = nullptr;
						AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
						if (Foreuse)
						{
							UGameManager::GetInstance()->PlaySound("SND_Foreuse_Grab");
						}
					}

					else if (Item->GetCurrentNode()->IsPlayerOnNeighbouringNodeWithoutOwnershipTransfer())
					{
						//L'Objet est un Item on l'�quipe au Joueur
						UE_LOG(LogTemp, Error, TEXT("Equipping foreuse!"));
						PlayerFinal->HeldItem = Item;
						Item->SetIsHeld();
					}
					AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
					if (Foreuse)
					{
						UGameManager::GetInstance()->PlaySound("SND_Foreuse_Grab");
					}
				}
				AWall* Wall = Cast<AWall>(HitResult.GetActor());
				if (Wall)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit a wall"));
					//C'est un Wall
					if (PlayerFinal->HeldItem)
					{
						//Le Joueur a un Item
						AForeuse* Foreuse = Cast<AForeuse>(PlayerFinal->HeldItem);
						if (Foreuse && Foreuse->Usable)
						{
							//C'est une Foreuse et elle est utilisable
							if ((Wall->GetActorLocation().X == Foreuse->GetActorLocation().X && Foreuse->GetActorLocation().X == PlayerFinal->GetActorLocation().X) || (Wall->GetActorLocation().Y == Foreuse->GetActorLocation().Y && Foreuse->GetActorLocation().Y == PlayerFinal->GetActorLocation().Y))
							{
								UE_LOG(LogTemp, Error, TEXT("Mur Foreuse Joueur aligné"));
								//La Foreuse, Le Wall et le Joueur sont bien align�s
								for (int i = 0; i < Wall->ConnectionBlockedNodes.Num(); ++i)
								{
									if (Wall->ConnectionBlockedNodes[i]->IsForeuseOnNeighbouringNode())
									{
										//La Foreuse est bien � c�t� du Wall
										PlayerFinal->InitiateMovement(Foreuse->CurrentNode);
										//Foreuse->SetForeuseLocation(Wall->CurrentNode, FVector(NULL, NULL, NULL));
										Foreuse->SetIsHeld();
										PlayerFinal->HeldItem = nullptr;
										Foreuse->ItemEffect(Wall);
										UGameManager::GetInstance()->PlaySound("SND_ForeuseEnd");
										return;
									}
								}
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
	InputComponent->BindAction("PressF", IE_Pressed, this, &ACasaPlayerController::OnPressF);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ACasaPlayerController::OnReload);
	InputComponent->BindAction("Load", IE_Pressed, this, &ACasaPlayerController::OnLoadNextLevel);
}

void ACasaPlayerController::OnReload()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetMapName()));
}

void ACasaPlayerController::OnLoadNextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), UGameManager::GetInstance()->NextLevel);
}

void ACasaPlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	InputComponent->ClearActionBindings();
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