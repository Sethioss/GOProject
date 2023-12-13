// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PathActor.h"

// D�finissez une fonction pour g�rer le clic de souris
FVector	WorldPosition, WorldDirection;

void ACasaPlayerController::OnMouseClick()
{
	ACasaPlayer* PlayerFinal = Cast<ACasaPlayer>(GetPawn());
	if (PlayerFinal)
	{
		// R�cup�rez la position de la souris
		FVector2D MousePosition;
		GetMousePosition(MousePosition.X, MousePosition.Y);

		// Maintenant, vous avez les coordonn�es X et Y de la souris dans MousePosition
		float MouseX = MousePosition.X;
		float MouseY = MousePosition.Y;

		FHitResult HitResult;

		FVector2D MousePos(MouseX, MouseY);

		// Faites quelque chose avec les coordonn�es (par exemple, tracez-les dans la console)
		//UE_LOG(LogTemp, Warning, TEXT("Coordonn�es de la souris - X: %f, Y: %f"), MouseX, MouseY);

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
					//UE_LOG(LogTemp, Warning, TEXT("It's a path with a node"));

					if (Path->IsPlayerOnNeighbouringNode())
					{
						FVector2D ActorLocation(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y);
						//MousePosition.X = HitResult.GetActor()->GetActorLocation().X;
						//MousePosition.Y = HitResult.GetActor()->GetActorLocation().Y;

						PlayerFinal->MoveTo(ActorLocation);
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