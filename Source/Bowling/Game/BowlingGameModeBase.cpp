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
}

void ABowlingGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPinMovement(DeltaTime);
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

	if (FrameScores->Num() < 10 && (FrameScores->Num() == 0 || (FrameScores->Last()->SecondBowl != -1 || FrameScores->Last()->FirstBowl == 10)))
	{
		FrameScores->Add(new BowlingFrameScore());
	}

	if (FrameScores->Last()->FirstBowl == -1)
	{
		FrameScores->Last()->FirstBowl = NumberOfPinsDown;

		// If it's a strike
		if (NumberOfPinsDown == 10)
		{
			// We're in the last frame, need to reset the pins for last two bowls
			if (FrameScores->Num() == 10)
			{
				PinsShouldBeReset = true;
			}
			else
			{
				// Normal frame
				PlayerShouldChange = true;
			}
		}
	}
	else if (FrameScores->Last()->SecondBowl == -1)
	{
		// We're in the last frame...
		if (FrameScores->Num() == 10)
		{
			// There was a strike on the first bowl, we just score this as normal
			if (FrameScores->Last()->FirstBowl == 10)
			{
				FrameScores->Last()->SecondBowl = NumberOfPinsDown;

				// If it's a strike
				if (NumberOfPinsDown == 10)
				{
					PinsShouldBeReset = true;
				}
			}
			// Otherwise it's number of pins down minus the first bowl as normal
			else
			{
				FrameScores->Last()->SecondBowl = NumberOfPinsDown - FrameScores->Last()->FirstBowl;

				// If this is a spare, need to reset the pins
				if (FrameScores->Last()->FirstBowl + FrameScores->Last()->SecondBowl == 10)
				{
					PinsShouldBeReset = true;
				}
			}
		}
		else
		{
			// Normal frame
			FrameScores->Last()->SecondBowl = NumberOfPinsDown - FrameScores->Last()->FirstBowl;
			PlayerShouldChange = true;
		}
	}
	else if (FrameScores->Last()->ThirdBowl == -1 && FrameScores->Num() == 10)
	{
		FrameScores->Last()->ThirdBowl = NumberOfPinsDown;
	}
}

void ABowlingGameModeBase::UpdateTotalScore(TArray<BowlingFrameScore*>* FrameScores)
{
	// Loop through all and check for bonuses

	for (int32 i = 0; i < FrameScores->Num(); i++)
	{
		if (i == 9)
		{
			ScoreTheEndFrame(FrameScores);
			break;
		}

		// If the total score is already calculated, skip
		if ((*FrameScores)[i]->TotalRunningScore != -1) { continue; }

		// If it's a strike, check for first bowl in the frame ahead, if existing
		if ((*FrameScores)[i]->FirstBowl == 10)
		{
			if (ScoreAStrikeFrame(i, FrameScores)) { continue; }

			break;
		}

		// If the first bowl is done but not a strike, and the second not done, we can't score it yet
		if ((*FrameScores)[i]->SecondBowl == -1)
		{
			break;
		}

		// If it's a spare need to check the first bowl in the frame ahead
		if ((*FrameScores)[i]->FirstBowl + (*FrameScores)[i]->SecondBowl == 10)
		{
			if (ScoreASpareFrame(i, FrameScores)) { continue; }

			break;
		}

		ScoreAnOpenFrame(i, FrameScores);
	}
}

void ABowlingGameModeBase::ScoreAnOpenFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores)
{
	int32 PreviousTotal = FrameIndex == 0 ? 0 : (*FrameScores)[FrameIndex - 1]->TotalRunningScore;

	(*FrameScores)[FrameIndex]->TotalRunningScore = PreviousTotal + (*FrameScores)[FrameIndex]->FirstBowl + (*FrameScores)[FrameIndex]->SecondBowl;
}

bool ABowlingGameModeBase::ScoreASpareFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores)
{
	int32 PreviousTotal = FrameIndex == 0 ? 0 : (*FrameScores)[FrameIndex - 1]->TotalRunningScore;

	if (FrameScores->Num() > FrameIndex + 1)
	{
		(*FrameScores)[FrameIndex]->TotalRunningScore = PreviousTotal + 10 + (*FrameScores)[FrameIndex + 1]->FirstBowl;
		return true;
	}

	return false;
}

bool ABowlingGameModeBase::ScoreAStrikeFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores)
{
	int32 PreviousTotal = FrameIndex == 0 ? 0 : (*FrameScores)[FrameIndex - 1]->TotalRunningScore;

	if (FrameScores->Num() > FrameIndex + 1)
	{
		// If there was a strike in the following frame we also need to check if the next next frame exists in order to score the bonus
		if ((*FrameScores)[FrameIndex + 1]->FirstBowl == 10 && !(FrameIndex + 1 == 9))
		{
			if (FrameScores->Num() > FrameIndex + 2)
			{
				(*FrameScores)[FrameIndex]->TotalRunningScore = PreviousTotal + 10 + 10 + (*FrameScores)[FrameIndex + 2]->FirstBowl;
				return true;
			}

			return false;
		}

		// If the second bowl in the following frame doesn't exist yet, we don't score it yet
		if ((*FrameScores)[FrameIndex + 1]->SecondBowl == -1)
		{
			return true;
		}

		(*FrameScores)[FrameIndex]->TotalRunningScore = PreviousTotal + 10 + (*FrameScores)[FrameIndex + 1]->FirstBowl + (*FrameScores)[FrameIndex + 1]->SecondBowl;
		return true;
	}

	return false;
}

void ABowlingGameModeBase::ScoreTheEndFrame(TArray<BowlingFrameScore*>* FrameScores)
{
	int32 PreviousTotal = (*FrameScores)[8]->TotalRunningScore;

	// If we have completed the second bowl and it's an open frame, we can score and complete now
	if ((*FrameScores)[9]->SecondBowl != -1 && ((*FrameScores)[9]->FirstBowl + (*FrameScores)[9]->SecondBowl) < 10)
	{
		(*FrameScores)[9]->TotalRunningScore = PreviousTotal + (*FrameScores)[9]->FirstBowl + (*FrameScores)[9]->SecondBowl;

		if (CurrentPlayer == MainMenuWidget->NumberOfPlayers - 1)
		{
			ChangeState(static_cast<uint8>(BowlingState::GameEnded));
		}
		else
		{
			PlayerShouldChange = true;
		}
	}

	// Otherwise if we have completed all three bowls we can score and complete now
	if ((*FrameScores)[9]->ThirdBowl != -1)
	{
		// If the first bowl was a strike, we do 10 plus second and third bowls
		if ((*FrameScores)[9]->FirstBowl == 10)
		{
			(*FrameScores)[9]->TotalRunningScore = PreviousTotal + 10 + (*FrameScores)[9]->SecondBowl + (*FrameScores)[9]->ThirdBowl;

			if (CurrentPlayer == MainMenuWidget->NumberOfPlayers - 1)
			{
				ChangeState(static_cast<uint8>(BowlingState::GameEnded));
			}
			else
			{
				PlayerShouldChange = true;
			}

			return;
		}

		// Otherwise it must be a spare
		(*FrameScores)[9]->TotalRunningScore = PreviousTotal + 10 + (*FrameScores)[9]->ThirdBowl;

		if (CurrentPlayer == MainMenuWidget->NumberOfPlayers - 1)
		{
			ChangeState(static_cast<uint8>(BowlingState::GameEnded));
		}
		else
		{
			PlayerShouldChange = true;
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

void ABowlingGameModeBase::ResetAllPinsForNewRound()
{
	// Reset all of the pins
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		BowlingPin->ReadyPinForNewRound();
	}
}

void ABowlingGameModeBase::ResetPinsToOriginalPosition()
{
	// Reset all of the pins' position
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);

		if (!BowlingPin->IsStanding())
		{
			BowlingPin->ResetPinToOriginalPositionAndHide();
		}
		else
		{
			BowlingPin->ResetToOriginalPositionAndRotation();
		}
	}
}

void ABowlingGameModeBase::CheckPinMovement(float DeltaTime)
{
	if (!(BowlingState == BowlingState::CheckPinsHaveStoppedMoving) || PinsBeingChecked) { return; }

	TimeSinceLastPinMovementCheck += DeltaTime;

	if (TimeSinceLastPinMovementCheck >= TimeBetweenPinMovementCheck)
	{
		TimeSinceLastPinMovementCheck = 0.0f;
	}
	else
	{
		return;
	}

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

void ABowlingGameModeBase::EnablePinsPhysicsAndDetachFromParent()
{
	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		BowlingPin->DetachFromParent();

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
	if (CurrentPlayer < 0 || CurrentPlayer >= Players.Num() || BowlingWidget == nullptr)
	{
		return;
	}

	TArray <BowlingPlayer*> PlayersToShow;
	PlayersToShow.Add(Players[CurrentPlayer]);

	BowlingWidget->ShowScorecards(PlayersToShow);

}

void ABowlingGameModeBase::ShowAllPlayersScorecards()
{
	if (BowlingWidget == nullptr)
	{
		return;
	}

	BowlingWidget->ShowScorecards(Players);
}

void ABowlingGameModeBase::ShowEndPlayersScorecardAndGameOverText()
{
	if (BowlingWidget)
	{
		BowlingWidget->ShowGameOverText();

		Players.Sort([](BowlingPlayer& A, BowlingPlayer& B) { return A.GetRunningScoreForFinalFrame() > B.GetRunningScoreForFinalFrame(); });

		BowlingWidget->ShowScorecards(Players);
	}
}

void ABowlingGameModeBase::NextPlayer()
{
	// ResetPins

	CurrentPlayer = CurrentPlayer == Players.Num() - 1 ? 0 : CurrentPlayer + 1;

	ShowCurrentPlayerScorecard();
}

void ABowlingGameModeBase::EndGame()
{
	ShowEndPlayersScorecardAndGameOverText();
}

// State stuff

void ABowlingGameModeBase::ChangeState(uint8 BowlingStateIndex)
{
	enum BowlingState NewState = static_cast<enum BowlingState>(BowlingStateIndex);

	// If we are in GameEnded state, only new game is allowed
	if (BowlingState == BowlingState::GameEnded && NewState != BowlingState::NewGame) { return; }

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

	case BowlingState::GameEnded:
		break;

	default:
		break;
	}
}

void ABowlingGameModeBase::NewGame_Implementation()
{
	Players.Empty();

	// Add players 	
	for (int32 i = 0; i < MainMenuWidget->NumberOfPlayers; i++)
	{
		Players.Add(new BowlingPlayer(FString::Printf(TEXT("Player %i"), i + 1)));
	}

	ShowCurrentPlayerScorecard();
}

void ABowlingGameModeBase::ReadyToBowl_Implementation()
{
	EnablePinsPhysicsAndDetachFromParent();
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
	SaveScores();
	if (BowlingState != BowlingState::GameEnded)
	{
		ShowCurrentPlayerScorecard();
		ChangeState(static_cast<uint8>(BowlingState::Sweep));
	}
	else
	{
		EndGame();
	}
}

void ABowlingGameModeBase::SweepState_Implementation()
{
	// Disable pin physics for the pins that need to remain standing
	DisablePinsPhysicsForStandingPins();
}

void ABowlingGameModeBase::CheckChangePlayerState_Implementation()
{
	ResetPinsToOriginalPosition();

	// If we should change to the next player, we need to reset the pins
	if (PlayerShouldChange)
	{
		NextPlayer();
		ResetAllPinsForNewRound();
	}

	if (PinsShouldBeReset)
	{
		ResetAllPinsForNewRound();
	}

	PlayerShouldChange = false;
	PinsShouldBeReset = false;

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

// TODO: Dev stuff maybe delete later
void ABowlingGameModeBase::KnockPinsDown(int32 NumberOfPins)
{
	for (int32 i = 0; i < NumberOfPins; i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		BowlingPin->KnockPinDown();
	}
}
