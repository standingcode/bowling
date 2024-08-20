// Fill out your copyright notice in the Description page of Project Settings.

#include "BowlingPin.h"

void ABowlingPin::BeginPlay()
{
	OriginalParent = Mesh->GetAttachmentRoot();

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

void ABowlingPin::ResetPinToOriginalPositionAndHide()
{
	PinFellOffEdge = true;
	DisableCollisionsAndPhysics();
	ResetToOriginalPositionAndRotation();
	HidePin();
}

void ABowlingPin::ReadyPinForNewRound()
{
	PinFellOffEdge = false;
	ShowPin();
}

void ABowlingPin::ResetToOriginalPositionAndRotation()
{
	Mesh->AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepWorldTransform);

	AActor* Parent = GetAttachParentActor();

	if (Parent == nullptr)
	{
		return;
	}

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
		ResetPinToOriginalPositionAndHide();
	}
}

float ABowlingPin::GetRootZOffsetComparedToOriginalLocation()
{
	AActor* Parent = GetAttachParentActor();

	if (Parent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Was null")));
		return 0.0f;
	}

	return Parent->GetTransform().GetLocation().Z - RootOriginalLocationZ;
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
	AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepWorldTransform);
}

void ABowlingPin::DetachFromParent()
{
	DetachRootComponentFromParent();
}

// TODO: Dev stuff maybe delete later
void ABowlingPin::KnockPinDown()
{
	PinFellOffEdge = true;
	ResetPinToOriginalPositionAndHide();
}

void ABowlingPin::EnableCollisionsAndPhysics()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
