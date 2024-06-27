// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Bowling\Item\BowlingPin.h"

void ABowlingGameModeBase::BeginPlay()
{
	// Get all of the bowling pins into the array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowlingPin::StaticClass(), BowlingPins);

	Super::BeginPlay();
}

void ABowlingGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPinMovement();
}

void ABowlingGameModeBase::ShowResultsOfBowl()
{
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		GEngine->AddOnScreenDebugMessage(i, 10.0f, FColor::White, FString::Printf(
			TEXT("Pin: %i is %s"),
			BowlingPin->GetPinNumber(),
			BowlingPin->IsStanding() ? TEXT("Standing") : TEXT("Down"))
		);
	}
}

void ABowlingGameModeBase::CheckPinMovement()
{
	if (!(BowlingState == BowlingState::CheckState) || PinsBeingChecked) { return; }

	PinsBeingChecked = true;

	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);

		if (BowlingPin->DidFallOffEdge())
		{
			continue;
		}

		if (!BowlingPin->GetVelocity().IsNearlyZero())
		{
			PinsBeingChecked = false;
			return;
		}
	}

	ChangeState(static_cast<uint8>(BowlingState::CheckState));
}

void ABowlingGameModeBase::AnalyseState()
{
}

void ABowlingGameModeBase::EnablePinsPhysics()
{
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);

		BowlingPin->EnableCollisions();
	}
}


// State stuff

void ABowlingGameModeBase::ChangeState(uint8 BowlingStateIndex)
{
	enum BowlingState NewState = static_cast<enum BowlingState>(BowlingStateIndex);
	BowlingState = NewState;

	switch (NewState)
	{
	case BowlingState::NewGame:
		// This might be implemented later
		break;
	case BowlingState::ReadyToBowl:
		EnablePinsPhysics();
		break;
	case BowlingState::PlayerHasLaunchedBall:
		break;
	case BowlingState::BallInMotion:
		break;
	case BowlingState::CheckState:
		ChangeState(static_cast<uint8>(BowlingState::SweepState));
		break;
	case BowlingState::SweepState:
		SweepState();
		break;
	case BowlingState::ResetState:
		break;

	default:
		break;
	}
}

void ABowlingGameModeBase::ReadyToBowl_Implementation()
{
	ChangeState(static_cast<uint8>(BowlingState::ReadyToBowl));
}

void ABowlingGameModeBase::PlayerHasLaunchedBall_Implementation()
{

}

void ABowlingGameModeBase::BallInMotion_Implementation()
{

}

void ABowlingGameModeBase::CheckPinsHaveStoppedMoving_Implementation()
{

}

void ABowlingGameModeBase::CheckState_Implementation()
{

}

void ABowlingGameModeBase::SweepState_Implementation()
{

}

void ABowlingGameModeBase::ResetState_Implementation()
{

}
