// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"

// D�finissez une fonction pour g�rer le clic de souris
void ACasaPlayerController::OnMouseClick()
{
	// R�cup�rez la position de la souris
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	// Maintenant, vous avez les coordonn�es X et Y de la souris dans MousePosition
	float MouseX = MousePosition.X;
	float MouseY = MousePosition.Y;

	// Faites quelque chose avec les coordonn�es (par exemple, tracez-les dans la console)
	UE_LOG(LogTemp, Warning, TEXT("Coordonn�es de la souris - X: %f, Y: %f"), MouseX, MouseY);

	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(CasaPlayer);
	if (PlayerFinal)
	{
		PlayerFinal->MoveTo(MousePosition);
	}

}

// Associez cette fonction � un �v�nement de clic de souris, par exemple, dans votre constructeur
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