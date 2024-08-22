// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingBall.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "BowlingGameModeBase.h"

// Sets default values
ABowlingBall::ABowlingBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Audio = CreateDefaultSubobject<UAudioComponent>("Audio");

	// Set the Root Component to be our Mesh
	RootComponent = Mesh;

	// Attach the audio component to the mesh
	Audio->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
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

	HideBall();

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
	InputComponent->BindAction("Quit", IE_Pressed, this, &ABowlingBall::Quit);

	// TODO: Dev stuff maybe delete later
	//InputComponent->BindAction("one", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<1>);
	//InputComponent->BindAction("two", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<2>);
	//InputComponent->BindAction("three", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<3>);
	//InputComponent->BindAction("four", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<4>);
	//InputComponent->BindAction("five", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<5>);
	//InputComponent->BindAction("six", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<6>);
	//InputComponent->BindAction("seven", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<7>);
	//InputComponent->BindAction("eight", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<8>);
	//InputComponent->BindAction("nine", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<9>);
	//InputComponent->BindAction("ten", IE_Pressed, this, &ABowlingBall::DevKnockPinsDown<10>);
}

// TODO: Dev stuff maybe delete later
void ABowlingBall::DevKnockPinsDown(int32 NumberOfPins)
{
	GameMode->KnockPinsDown(NumberOfPins);
	ReportBallOffEdgeOrStoppedMoving();
}

void ABowlingBall::MoveLeftAndRight(float Value)
{
	if (!(GameMode->BowlingState == BowlingState::ReadyToBowl)) { return; }

	// Move the player left and right
	FVector NewLocation = GetActorLocation();
	NewLocation.Y += Value * SidewaysSpeed;

	NewLocation.Y = FMath::Clamp(NewLocation.Y, -MoveLeftAndRightLimit, MoveLeftAndRightLimit);

	SetActorLocation(NewLocation);
}

void ABowlingBall::Bowl()
{
	if (!(GameMode->BowlingState == BowlingState::ReadyToBowl)) { return; }

	GameMode->ChangeState(static_cast<uint8>(BowlingState::PlayerHasLaunchedBall));

	EnableCollisions();

	Mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
	Mesh->AddImpulse(FVector(BowlingForce, 0, 0));

	PlayerBowled();
	//Audio->Play();
}

void ABowlingBall::PlayerBowled_Implementation()
{
}

void ABowlingBall::Quit()
{
	FGenericPlatformMisc::RequestExit(false);
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
	Audio->Stop();
}

void ABowlingBall::ResetBallPosition()
{
	// Set the mesh velocity to 0
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
	Mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	SetActorLocation(InitialBallPosition);
	SetActorRotation(FRotator(0, 0, 0));
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
