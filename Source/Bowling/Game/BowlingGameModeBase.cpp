// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Bowling\Item\BowlingPin.h"

void ABowlingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Get all of the bowling pins into the array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowlingPin::StaticClass(), BowlingPins);

	// Launch new game
	ChangeState(static_cast<uint8>(BowlingState::NewGame));
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
	if (!(BowlingState == BowlingState::CheckPinsHaveStoppedMoving) || PinsBeingChecked) { return; }

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

		if (!BowlingPin->DidFallOffEdge())
		{
			BowlingPin->EnableCollisionsAndPhysics();
		}
	}
}

void ABowlingGameModeBase::DisablePinsPhysicsForStandingPins()
{
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);

		if (BowlingPin->IsStanding())
		{
			BowlingPin->DisableCollisionsAndPhysics();
		}
	}
}


// State stuff

void ABowlingGameModeBase::ChangeState(uint8 BowlingStateIndex)
{
	enum BowlingState NewState = static_cast<enum BowlingState>(BowlingStateIndex);

	// Change the checkable state here
	BowlingState = NewState;

	switch (NewState)
	{
	case BowlingState::NewGame:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("New game")));
		NewGame();
		break;
	case BowlingState::ReadyToBowl:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Ready to bowl")));
		ReadyToBowl();
		break;
	case BowlingState::PlayerHasLaunchedBall:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Player has launched ball")));
		PlayerHasLaunchedBall();
		break;
	case BowlingState::BallInMotion:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Ball in motion")));
		BallInMotion();
		break;
	case BowlingState::CheckPinsHaveStoppedMoving:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Check pins have stopped moving")));
		CheckPinsHaveStoppedMoving();
		break;
	case BowlingState::CheckState:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Check state")));
		CheckState();
		break;
	case BowlingState::SweepState:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Sweep state")));
		SweepState();
		break;
	case BowlingState::ResetState:
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::White, FString::Printf(TEXT("Sweep state")));
		ResetState();
		break;
	default:
		break;
	}
}

void ABowlingGameModeBase::NewGame_Implementation()
{

}

void ABowlingGameModeBase::ReadyToBowl_Implementation()
{
	EnablePinsPhysics();
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
	ChangeState(static_cast<uint8>(BowlingState::SweepState));
}

void ABowlingGameModeBase::SweepState_Implementation()
{
	// Disable pin physics for the pins that need to remain standing
	DisablePinsPhysicsForStandingPins();

}

void ABowlingGameModeBase::ResetState_Implementation()
{

}
