// Fill out your copyright notice in the Description page of Project Settings.


#include "CasaPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
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

	MoverComponent = CreateDefaultSubobject<UMoverComponent>(TEXT("Mover"));

	//On "attache" la camera au player
	//PlayerCamera->SetupAttachment(RootComponent);
	//PlayerCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	//PlayerCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//Creation Springarmcomponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	//Attach springarm to root
	SpringArmComponent->SetupAttachment(StaticMeshComponent);
	MoverComponent->SetupAttachment(StaticMeshComponent);

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

	RegisterToManager();
}

void ACasaPlayer::RegisterToManager()
{
	UGameManager::GetInstance()->Player = this;
	RegisteredToManager = true;
	CurrentNode = SnapToGrid();
	UGameManager::GetInstance()->ElementsToRegister -= 1;
	MoverComponent->Setup(FVector(500, 500, 500));
}

// Called every frame
void ACasaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (ShouldMove)
	{
		if(FVector::Distance(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
							 FVector(TargetedPosition.X, TargetedPosition.Y, GetActorLocation().Z))
			< 0.5f)
		{
			ShouldMove = false;
		}
		else
		{
			MoveToTarget(TargetedPosition, Speed);
		}
	}*/

}

// Called to bind functionality to input
void ACasaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

/*void ACasaPlayer::MoveToTarget(FVector2D Target, float speed) {

	FVector2D Direction = Target - FVector2D(GetActorLocation().X, GetActorLocation().Y).Normalize();

	AddActorLocalOffset(FVector(Direction.X * Speed * GetWorld()->GetDeltaSeconds(), Direction.Y * Speed * GetWorld()->GetDeltaSeconds(), 0));
}*/

void ACasaPlayer::MoveTo(FVector2D TargetPosition) {

	//TargetedPosition = TargetPosition;
	//ShouldMove = true;

	//FVector NewLocation(TargetPosition.X, TargetPosition.Y, GetActorLocation().Z);
	//SetActorLocation(NewLocation);

	//TurnFinished = true;
}

void ACasaPlayer::MoveTo(APathActor* TargetPosition) {

	//TargetedPosition = TargetPosition;
	//ShouldMove = true;

	//FVector NewLocation(TargetPosition->GetActorLocation().X, TargetPosition->GetActorLocation().Y, GetActorLocation().Z);
	//SetActorLocation(NewLocation);

	//TurnFinished = true;
}

void ACasaPlayer::InitiateMovement(APathActor* Target)
{
	UGameManager::GetInstance()->PlayerNextNode = Target;
	UGameManager::GetInstance()->GetFsm()->ChangeState("OnPrePlayerTurn");
}

void ACasaPlayer::InitiateMovementWithDrill(APathActor* Target, APathActor* ForeuseTarget)
{
	UGameManager::GetInstance()->PlayerNextNode = Target;
	UGameManager::GetInstance()->Player->HeldItem->NextNode = ForeuseTarget;
	UGameManager::GetInstance()->GetFsm()->ChangeState("OnPrePlayerTurn");
}

APathActor* ACasaPlayer::SnapToGrid(FVector offset)
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

			SetActorLocation(FVector(Path->GetActorLocation().X, Path->GetActorLocation().Y, Path->GetActorLocation().Z + (ActorBounds.GetSize().Z / 2)));
		}
	}

	return CurrentNode;
}

