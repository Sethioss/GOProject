// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"

// Définissez une fonction pour gérer le clic de souris
void ACasaPlayerController::OnMouseClick()
{
	// Récupérez la position de la souris
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	// Maintenant, vous avez les coordonnées X et Y de la souris dans MousePosition
	float MouseX = MousePosition.X;
	float MouseY = MousePosition.Y;

	// Faites quelque chose avec les coordonnées (par exemple, tracez-les dans la console)
	UE_LOG(LogTemp, Warning, TEXT("Coordonnées de la souris - X: %f, Y: %f"), MouseX, MouseY);

	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());
	if (PlayerFinal)
	{
		
		FVector	WorldPosition, WorldDirection;

		APlayerController* MyController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

		if (MyController) {
			MyController->DeprojectScreenPositionToWorld(MouseX,MouseY,WorldPosition, WorldDirection);
		}
		UE_LOG(LogTemp, Warning, TEXT("WorldLocation: %s"), *WorldPosition.ToString());
		MousePosition.X = WorldPosition.X;
		MousePosition.Y = WorldPosition.Z;

		PlayerFinal->MoveTo(MousePosition);
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
}