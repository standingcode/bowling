// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABowlingPlayer::ABowlingPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Set the Root Component to be our Mesh
	RootComponent = Mesh;
	// Attach the Spring Arm to the Mesh, spring will now follow the mesh transform
	SpringArm->SetupAttachment(Mesh);
	// Attach the Camera to the Spring Arm, camera will now follow the spring arm transform
	Camera->SetupAttachment(SpringArm);

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
	GEngine->AddOnScreenDebugMessage(-3, 0.5f, FColor::White, FString::Printf(TEXT("Got here and power is: %f"), BowlingForce));
	Mesh->AddImpulse(FVector(BowlingForce, 0, 0));
}
