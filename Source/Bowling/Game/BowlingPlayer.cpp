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

}

void ABowlingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check the current speed of the ball
	CheckCurrentBallSpeed(Mesh->GetComponentVelocity());

}

// Called to bind functionality to input
void ABowlingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Input Axis Bindings	
	InputComponent->BindAxis("MoveLeftAndRight", this, &ABowlingPlayer::MoveLeftAndRight);
	InputComponent->BindAction("Bowl", IE_Pressed, this, &ABowlingPlayer::Bowl);

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
	if (!CanBowl) { return; }

	CanBowl = false;
	//GEngine->AddOnScreenDebugMessage(-3, 0.5f, FColor::White, FString::Printf(TEXT("Got here and power is: %f"), BowlingForce));
	Mesh->AddImpulse(FVector(BowlingForce, 0, 0));
}

void ABowlingPlayer::BallHasStopped()
{
	ABowlingGameModeBase* GameMode = Cast<ABowlingGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->BowlFinished();
}

void ABowlingPlayer::CheckCurrentBallSpeed(FVector Velocity)
{
	if (CanBowl) { return; }

	if (!StartCheckingForBallStopping)
	{
		if (Velocity.X > 0.01f)
		{
			StartCheckingForBallStopping = true;
		}

		return;
	}

	// If the velocity is less than 0.1, the ball has stopped
	if (Velocity.X < 0.01f)
	{
		BallHasStopped();
	}
}
