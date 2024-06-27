// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BowlingGameModeBase.h"

// Sets default values
ABowlingPlayer::ABowlingPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Set the Root Component to be our Mesh
	RootComponent = Mesh;

	// Set physics to true
	Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABowlingPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Account for mass in the bowling force
	BowlingForce *= Mesh->GetMass();

	// Reference to the GameModeBase script
	GameMode = Cast<ABowlingGameModeBase>(GetWorld()->GetAuthGameMode());
}

void ABowlingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCurrentBallSpeed(Mesh->GetComponentVelocity());
	CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(GetActorLocation().Z);
}

// Called to bind functionality to input
void ABowlingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Input Axis Bindings	
	InputComponent->BindAxis("MoveLeftAndRight", this, &ABowlingPlayer::MoveLeftAndRight);
	InputComponent->BindAction("Bowl", IE_Pressed, this, &ABowlingPlayer::Bowl);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ABowlingPlayer::Reset);
}

void ABowlingPlayer::MoveLeftAndRight(float Value)
{
	// Move the player left and right
	FVector NewLocation = GetActorLocation();
	NewLocation.Y += Value * SidewaysSpeed;
	SetActorLocation(NewLocation);
}

void ABowlingPlayer::Bowl()
{
	if (!GameMode->CanBowl) { return; }

	GameMode->CanBowl = false;
	BallIsInMotion = true;
	//GEngine->AddOnScreenDebugMessage(-3, 0.5f, FColor::White, FString::Printf(TEXT("Got here and power is: %f"), BowlingForce));
	Mesh->AddImpulse(FVector(BowlingForce, 0, 0));
}

void ABowlingPlayer::CheckCurrentBallSpeed(FVector Velocity)
{
	if (Velocity.X > 0.01f)
	{
		BallIsInMotion = true;
	}
	else
	{
		BallIsInMotion = false;
	}

	if (GameMode->CanBowl || !BallIsInMotion) { return; }

	// If the velocity is less than 0.1, the ball has stopped
	if (Velocity.X < 0.01f)
	{
		ReportBallOffEdgeOrStoppedMoving();
	}
}

void ABowlingPlayer::CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(float ZPosition)
{
	if (GameMode->CanBowl || !BallIsInMotion) { return; }

	if (ZPosition < 0)
	{
		ReportBallOffEdgeOrStoppedMoving();
	}
}

void ABowlingPlayer::ReportBallOffEdgeOrStoppedMoving()
{	
	DisableCollisions();
	BallIsInMotion = false;
	GameMode->BallReportedStoppedOrOffTheEdge();
}

void ABowlingPlayer::Reset()
{
	GameMode->Reset();
}

void ABowlingPlayer::ShowBall()
{
	SetActorHiddenInGame(false);
}

void ABowlingPlayer::HideBall()
{
	SetActorHiddenInGame(true);
}

void ABowlingPlayer::EnableCollisions()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABowlingPlayer::DisableCollisions()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
