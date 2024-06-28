// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

// Sets default values
ABowlingPin::ABowlingPin()
{

}

void ABowlingPin::BeginPlay()
{
	Parent = GetAttachParentActor();

	Super::BeginPlay();
	OriginalPosition = GetActorLocation();
	OriginalRotation = GetActorRotation();
}

void ABowlingPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckIfPinFellOffEdge();
}

#pragma region Resetting

void ABowlingPin::ResetPin()
{
	DisableCollisionsAndPhysics();
	ResetToOriginalPositionAndRotation();
	//HidePin();

}

void ABowlingPin::ResetToOriginalPositionAndRotation()
{
	SetActorLocation(OriginalPosition);
	SetActorRotation(OriginalRotation);
}

#pragma endregion

int ABowlingPin::GetPinNumber()
{
	return PinNumber;
}

bool ABowlingPin::IsStanding()
{
	// The pin might be falling through the universe, if so it is not standing, return false
	if (DidFallOffEdge()) { return false; }

	// Is the pin standing up?
	return GetActorUpVector().Z > MinimumValueToConsiderStillStanding;
}

bool ABowlingPin::DidFallOffEdge()
{
	return PinFellOffEdge;
}

void ABowlingPin::CheckIfPinFellOffEdge()
{
	if (GetActorLocation().Z < 0.0f)
	{
		PinFellOffEdge = true;
		ResetPin();
	}
}

void ABowlingPin::HidePin()
{
	SetActorHiddenInGame(true);
}

void ABowlingPin::ShowPin()
{
	SetActorHiddenInGame(false);
}

void ABowlingPin::DisableCollisionsAndPhysics()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
}

void ABowlingPin::EnableCollisionsAndPhysics()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
