// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingBall.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BowlingGameModeBase.h"

// Sets default values
ABowlingBall::ABowlingBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Set the Root Component to be our Mesh
	RootComponent = Mesh;

	//// Set physics to false at the start
	//Mesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABowlingBall::BeginPlay()
{
	Super::BeginPlay();

	// Account for mass in the bowling force
	Mesh->SetSimulatePhysics(true);
	BowlingForce *= Mesh->GetMass();
	Mesh->SetSimulatePhysics(false);
	InitialBallPosition = GetActorLocation();

	// Reference to the GameModeBase script
	GameMode = Cast<ABowlingGameModeBase>(GetWorld()->GetAuthGameMode());
}

void ABowlingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCurrentBallSpeed(Mesh->GetComponentVelocity());
	CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(GetActorLocation().Z);
}

// Called to bind functionality to input
void ABowlingBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Input Axis Bindings	
	InputComponent->BindAxis("MoveLeftAndRight", this, &ABowlingBall::MoveLeftAndRight);
	InputComponent->BindAction("Bowl", IE_Pressed, this, &ABowlingBall::Bowl);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ABowlingBall::Reset);
}

void ABowlingBall::MoveLeftAndRight(float Value)
{
	if (
		GameMode->BowlingState == BowlingState::BallInMotion
		|| GameMode->BowlingState == BowlingState::PlayerHasLaunchedBall
		)
	{
		return;
	}

	// Move the player left and right
	FVector NewLocation = GetActorLocation();
	NewLocation.Y += Value * SidewaysSpeed;

	NewLocation.Y = FMath::Clamp(NewLocation.Y, -MoveLeftAndRightLimit, MoveLeftAndRightLimit);

	SetActorLocation(NewLocation);
}

void ABowlingBall::Bowl()
{
	if (!(GameMode->BowlingState == BowlingState::ReadyToBowl)) { return; }

	EnableCollisions();

	GameMode->ChangeState(static_cast<uint8>(BowlingState::PlayerHasLaunchedBall));
	//GEngine->AddOnScreenDebugMessage(-3, 0.5f, FColor::White, FString::Printf(TEXT("Got here and power is: %f"), BowlingForce));
	Mesh->AddImpulse(FVector(BowlingForce, 0, 0));
}

void ABowlingBall::ResetBall()
{
	ResetBallPosition();
	ShowBall();
}

void ABowlingBall::CheckCurrentBallSpeed(FVector Velocity)
{
	if (GameMode->BowlingState == BowlingState::PlayerHasLaunchedBall)
	{
		if (Velocity.X > 0.05f)
		{
			GameMode->ChangeState(static_cast<uint8>(BowlingState::BallInMotion));
			return;
		}
	}

	if (GameMode->BowlingState == BowlingState::BallInMotion)
	{
		if (Velocity.X < 0.05f)
		{
			ReportBallOffEdgeOrStoppedMoving();
		}
	}
}

void ABowlingBall::CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(float ZPosition)
{
	if (!(GameMode->BowlingState == BowlingState::BallInMotion)) { return; }

	if (ZPosition < 1.0f)
	{
		HideBall();
		ReportBallOffEdgeOrStoppedMoving();
	}
}

void ABowlingBall::ReportBallOffEdgeOrStoppedMoving()
{
	DisableCollisions();
	GameMode->ChangeState(static_cast<uint8>(BowlingState::CheckPinsHaveStoppedMoving));
	Mesh->SetSimulatePhysics(false);
}

void ABowlingBall::ResetBallPosition()
{
	SetActorLocation(InitialBallPosition);
}

void ABowlingBall::ShowBall()
{
	SetActorHiddenInGame(false);
}

void ABowlingBall::HideBall()
{
	SetActorHiddenInGame(true);
}

void ABowlingBall::EnableCollisions()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABowlingBall::DisableCollisions()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
