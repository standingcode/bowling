// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingWidget.h"
#include "BowlingScorerComponent.h"
#include "BowlingPlayer.h"
#include "ScoringWidget.h"
#include "MainMenu.h"
#include "BowlingGameModeBase.generated.h"


UENUM(BlueprintType)
enum class BowlingState : uint8
{
	NewGame = 0 UMETA(DisplayName = "New game"),
	ReadyToBowl = 1 UMETA(DisplayName = "Ready to bowl"),
	PlayerHasLaunchedBall = 2 UMETA(DisplayName = "Player has launched ball"),
	BallInMotion = 3 UMETA(DisplayName = "Ball is in Motion"),
	CheckPinsHaveStoppedMoving = 4 UMETA(DisplayName = "Check pins have stopped moving"),
	AnalyseScore = 5 UMETA(DisplayName = "Analyse the score"),
	Sweep = 7 UMETA(DisplayName = "Sweep the pins"),
	DescendPins = 8 UMETA(DisplayName = "Descend the pins")
};

/**
 *
 */
UCLASS()
class BOWLING_API ABowlingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	// Functions
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	void SaveScores();
	void CheckPinMovement(float DeltaTime);
	void SaveBowlScore(TArray<BowlingFrameScore*>* FrameScores);
	void UpdateTotalScore(TArray<BowlingFrameScore*>* FrameScores);
	void ScoreAnOpenFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores);
	bool ScoreASpareFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores);
	bool ScoreAStrikeFrame(int32 FrameIndex, TArray<BowlingFrameScore*>* FrameScores);
	void ScoreTheEndFrame(TArray<BowlingFrameScore*>* FrameScores);
	void EnablePinsPhysics();
	void DisablePinsPhysicsForStandingPins();
	void ShowCurrentPlayerScorecard();
	void ShowEndPlayersScorecardAndGameOverText();
	int32 GetNumberOfPinsDown();
	void ResetAllPins();
	void ResetStandingPinsToOriginalPosition();
	void ResetNonStandingPinsToOriginalPosition();
	void EndGame();

	// Variables
	TArray<AActor*> BowlingPins;

	TArray<BowlingPlayer*> Players;

	bool PinsBeingChecked = false;
	bool PlayerShouldChange = false;
	bool PinsShouldBeReset = false;
	bool GameIsNotInPlay = true;

	float TimeSinceLastPinMovementCheck = 0.0f;
	float TimeBetweenPinMovementCheck = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBowlingWidget* BowlingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UBowlingScorerComponent* BowlingScorerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfPlayers = 1;

	int32 CurrentPlayer = 0;

	UFUNCTION()
	void NextPlayer();

public:

	BowlingState BowlingState = BowlingState::NewGame;

	UFUNCTION(BlueprintCallable)
	void ChangeState(uint8 BowlingStateIndex);

	// State events

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NewGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReadyToBowl();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayerHasLaunchedBall();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BallInMotion();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CheckPinsHaveStoppedMoving();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AnalyseScoreState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SweepState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CheckChangePlayerState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DescendPinsState();

	// TODO: Dev stuff maybe delete later
	void KnockPinsDown(int32 NumberOfPins);
};
