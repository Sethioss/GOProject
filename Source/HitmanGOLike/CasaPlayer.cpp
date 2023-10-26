// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CasaPlayerController.h"
#include "CasaPlayer.h"

// Sets default values
ACasaPlayer::ACasaPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation du component staticmesh
	StaticMeshComponent =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visuel"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(TEXT("/Engine/BasicShapes/Cone.Cone")); //static permet d'executer la fonction qu'une fois
	StaticMeshComponent->SetStaticMesh(meshFinder.Object);

	// creation du component audio
	AudioComponent =
		CreateDefaultSubobject<UAudioComponent>(TEXT("Son"));

	RootComponent = StaticMeshComponent;
	AudioComponent->SetupAttachment(RootComponent);

	// creation de la camera du player
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//On "attache" la camera au player
	//PlayerCamera->SetupAttachment(RootComponent);
	//PlayerCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	//PlayerCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//Creation Springarmcomponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	//Attach springarm to root
	SpringArmComponent->SetupAttachment(StaticMeshComponent);

	//Attach PlayerCamera to springarm
	PlayerCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	
	//Assign SpringArm class variables.
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;



}

// Called when the game starts or when spawned
void ACasaPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACasaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}

}

// Called to bind functionality to input
void ACasaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("CasaPlayerMoveX", this, &ACasaPlayer::Move_XAxis);
	InputComponent->BindAxis("CasaPlayerMoveY", this, &ACasaPlayer::Move_YAxis);

}

void ACasaPlayer::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -100.0f, 100.0f);
}

void ACasaPlayer::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -100.0f, 100.0f);
}

void ACasaPlayer::MoveTo(FVector2D TargetPosition) {

	//Converstion en FVector
	FVector NewLocation(TargetPosition.X,TargetPosition.Y,GetActorLocation().Z);

	//Changement des coordonnées
	SetActorLocation(NewLocation);

}

