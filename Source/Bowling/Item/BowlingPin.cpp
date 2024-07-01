// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingPin.h"

void ABowlingPin::BeginPlay()
{
	Parent = GetAttachParentActor();

	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	RootOriginalLocationZ = GetAttachParentActor()->GetTransform().GetLocation().Z;
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
	HidePin();
}

void ABowlingPin::ResetToOriginalPositionAndRotation()
{
	SetActorLocation(OriginalLocation + FVector(0.0f, 0.0f, GetRootZOffsetComparedToOriginalLocation()));
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

float ABowlingPin::GetRootZOffsetComparedToOriginalLocation()
{
	return GetAttachParentActor()->GetTransform().GetLocation().Z - RootOriginalLocationZ;
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
