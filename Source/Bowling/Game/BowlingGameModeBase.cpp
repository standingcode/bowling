// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Bowling\Item\BowlingPin.h"
#include "ScoringWidget.h"
#include "BowlingWidget.h"

void ABowlingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Get all of the bowling pins into the array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowlingPin::StaticClass(), BowlingPins);

	// Instantiate a new instance of bowling scorer object
	BowlingScorerComponent = NewObject<UBowlingScorerComponent>(this, TEXT("BowlingScorerComponent"));

	//BowlingScorerComponent->RegisterComponent();	

	// Add players 	
	for (int32 i = 0; i < NumberOfPlayers; i++)
	{
		Players.Add(new BowlingPlayer(FString::Printf(TEXT("Player %i"), i + 1)));
	}

	/*for (int32 i = 0; i < NumberOfPlayers; i++)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::White,
			FString::Printf(TEXT("Player name is: %s"), *Players[i]->GetName()));
	}*/


	// Launch new game
	ChangeState(static_cast<uint8>(BowlingState::NewGame));
}

void ABowlingGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPinMovement();
}

void ABowlingGameModeBase::SaveScoresAndUpdateVisualScorecard()
{
	/*for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		GEngine->AddOnScreenDebugMessage(i, 10.0f, FColor::White, FString::Printf(
			TEXT("Pin: %i is %s"),
			BowlingPin->GetPinNumber(),
			BowlingPin->IsStanding() ? TEXT("Standing") : TEXT("Down"))
		);
	}*/

	int32 NumberOfPinsDown = GetNumberOfPinsDown();

	// Save the score
	TArray<BowlingFrameScore*>* FrameScores = Players[CurrentPlayer]->GetAllFrameScores();

	if (FrameScores->Num() == 0 || FrameScores->Last()->SecondBowl != -1)
	{
		FrameScores->Add(new BowlingFrameScore());
	}

	if (FrameScores->Last()->FirstBowl == -1)
	{
		FrameScores->Last()->FirstBowl = NumberOfPinsDown;
		if (NumberOfPinsDown == 10) { NextPlayer(); }
	}
	else
	{
		FrameScores->Last()->SecondBowl = NumberOfPinsDown - FrameScores->Last()->FirstBowl;
		NextPlayer();
	}

	// Show current player scorecard
	ShowCurrentPlayerScorecard();
}

int32 ABowlingGameModeBase::GetNumberOfPinsDown()
{
	int32 NumberOfPinsDown = 0;

	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		if (!BowlingPin->IsStanding()) { NumberOfPinsDown++; }
	}

	return NumberOfPinsDown;
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

	PinsBeingChecked = false;
	ChangeState(static_cast<uint8>(BowlingState::CheckState));
}

void ABowlingGameModeBase::AnalyseScoreAndNextState()
{
	SaveScoresAndUpdateVisualScorecard();
	ChangeState(static_cast<uint8>(BowlingState::SweepState));
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

void ABowlingGameModeBase::ShowCurrentPlayerScorecard()
{
	if (BowlingWidget)
	{
		TArray <BowlingPlayer*> PlayersToShow;
		PlayersToShow.Add(Players[CurrentPlayer]);

		BowlingWidget->ShowScorecards(PlayersToShow);
	}
}

void ABowlingGameModeBase::NextPlayer()
{
	// ResetPins

	CurrentPlayer++;

	if (CurrentPlayer >= NumberOfPlayers - 1)
	{
		CurrentPlayer = 0;
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
	ShowCurrentPlayerScorecard();
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
	AnalyseScoreAndNextState();
}

void ABowlingGameModeBase::SweepState_Implementation()
{
	// Disable pin physics for the pins that need to remain standing
	DisablePinsPhysicsForStandingPins();

}

void ABowlingGameModeBase::ResetState_Implementation()
{

}
