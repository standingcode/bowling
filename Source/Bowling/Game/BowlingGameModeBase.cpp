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

void ABowlingGameModeBase::SaveScores()
{
	TArray<BowlingFrameScore*>* FrameScores = Players[CurrentPlayer]->GetAllFrameScores();

	SaveBowlScore(FrameScores);
	UpdateTotalScore(FrameScores);
}

void ABowlingGameModeBase::SaveBowlScore(TArray<BowlingFrameScore*>* FrameScores)
{
	int32 NumberOfPinsDown = GetNumberOfPinsDown();

	// Save the bowling score


	if (FrameScores->Num() == 0 || (FrameScores->Last()->SecondBowl != -1 || FrameScores->Last()->FirstBowl == 10))
	{
		FrameScores->Add(new BowlingFrameScore());
	}

	if (FrameScores->Last()->FirstBowl == -1)
	{
		FrameScores->Last()->FirstBowl = NumberOfPinsDown;
		if (NumberOfPinsDown == 10) { PlayerShouldChange = true; }
	}
	else
	{
		FrameScores->Last()->SecondBowl = NumberOfPinsDown - FrameScores->Last()->FirstBowl;
		PlayerShouldChange = true;
	}
}

void ABowlingGameModeBase::UpdateTotalScore(TArray<BowlingFrameScore*>* FrameScores)
{
	// Work backwards through the array to update bonuses etc.



	// If the score was less than 10, we can just add the two bowls together
	if (FrameScores->Last()->FirstBowl + FrameScores->Last()->SecondBowl < 10)
	{
		FrameScores->Last()->TotalRunningScore = FrameScores->Last()->FirstBowl + FrameScores->Last()->SecondBowl;
	}

	// Loop through all and check for bonuses

	for (int32 i = 0; i < FrameScores->Num(); i++)
	{
		if ((*FrameScores)[i]->TotalRunningScore != -1) { continue; }

		// If it's a strike, check for 2 bowls in the frame ahead, if existing
		if ((*FrameScores)[i]->FirstBowl == 10 && FrameScores->Num() > i + 1)
		{
			// If there was only a strike in this frame we also need to check if the next frame exists,
			// in order to score the bonus
			if ((*FrameScores)[i + 1]->FirstBowl == 10 && FrameScores->Num() > i + 2)
			{
				(*FrameScores)[i]->TotalRunningScore = FrameScores
			}
		}
	}
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

void ABowlingGameModeBase::ResetAllPins()
{
	// Reset all of the pins
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		BowlingPin->ReadyPinForNewRound();
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

	PinsBeingChecked = false;
	ChangeState(static_cast<uint8>(BowlingState::AnalyseScore));
}

void ABowlingGameModeBase::DetermineScore()
{
	SaveScores();
	ShowCurrentPlayerScorecard();
	ChangeState(static_cast<uint8>(BowlingState::Sweep));
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

	if (CurrentPlayer > NumberOfPlayers - 1)
	{
		CurrentPlayer = 0;
	}

	ShowCurrentPlayerScorecard();
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
		NewGame();
		break;

	case BowlingState::ReadyToBowl:
		ReadyToBowl();
		break;

	case BowlingState::PlayerHasLaunchedBall:
		PlayerHasLaunchedBall();
		break;

	case BowlingState::BallInMotion:
		BallInMotion();
		break;

	case BowlingState::CheckPinsHaveStoppedMoving:
		CheckPinsHaveStoppedMoving();
		break;

	case BowlingState::AnalyseScore:
		AnalyseScoreState();
		break;

	case BowlingState::Sweep:
		SweepState();
		break;
	case BowlingState::DescendPins:
		DescendPinsState();
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

void ABowlingGameModeBase::AnalyseScoreState_Implementation()
{
	DetermineScore();
}

void ABowlingGameModeBase::SweepState_Implementation()
{
	// Disable pin physics for the pins that need to remain standing
	DisablePinsPhysicsForStandingPins();

}

void ABowlingGameModeBase::CheckChangePlayerState_Implementation()
{
	// Can add a catch to check if any pins somehow got lost during the sweep here....

	// If we should change to the next player, we need to reset the pins
	if (PlayerShouldChange)
	{
		ResetAllPins();
		NextPlayer();
	}

	PlayerShouldChange = false;
	ChangeState(static_cast<uint8>(BowlingState::DescendPins));
}

void ABowlingGameModeBase::DescendPinsState_Implementation()
{

}

// Debug stuff

void Print(FString Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, Message);
}