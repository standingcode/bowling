// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingWidget.h"
#include "BowlingScorerComponent.h"
#include "BowlingGameModeBase.generated.h"


UENUM(BlueprintType)
enum class BowlingState : uint8
{
	NewGame = 0 UMETA(DisplayName = "New game"),
	ReadyToBowl = 1 UMETA(DisplayName = "Ready to bowl"),
	PlayerHasLaunchedBall = 2 UMETA(DisplayName = "Player has launched ball"),
	BallInMotion = 3 UMETA(DisplayName = "BallInMotion"),
	CheckPinsHaveStoppedMoving = 4 UMETA(DisplayName = "Check pins have stopped moving"),
	CheckState = 5 UMETA(DisplayName = "CheckState"),
	SweepState = 7 UMETA(DisplayName = "SweepState"),
	ResetState = 6 UMETA(DisplayName = "ResetState")
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

	void ShowResultsOfBowl();
	void CheckPinMovement();
	void AnalyseScoreAndNextState();
	void EnablePinsPhysics();
	void DisablePinsPhysicsForStandingPins();

	// Variables
	TArray<AActor*> BowlingPins;
	bool PinsBeingChecked = false;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> GameWidgetClass;

	UPROPERTY()
	UBowlingWidget* GameWidget;

	UPROPERTY()
	UBowlingScorerComponent* BowlingScorerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfPlayers = 1;

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
	void CheckState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SweepState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetState();
};
