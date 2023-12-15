// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

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

	HeldItem = nullptr;

}

// Called when the game starts or when spawned
void ACasaPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called every frame
void ACasaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACasaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACasaPlayer::MoveTo(FVector2D TargetPosition) 
{
	FVector NewLocation(TargetPosition.X, TargetPosition.Y, GetActorLocation().Z);
	SetActorLocation(NewLocation);
}

