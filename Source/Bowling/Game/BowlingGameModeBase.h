// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingGameModeBase.generated.h"

UENUM(BlueprintType)
enum class BowlingState : uint8
{
	NewGame = 0 UMETA(DisplayName = "New game"),
	ReadyToBowl = 1 UMETA(DisplayName = "Ready to bowl"),
	PlayerHasLaunchedBall = 2 UMETA(DisplayName = "Player has launched ball"),
	BallInMotion = 3 UMETA(DisplayName = "BallInMotion"),
	CheckState = 4 UMETA(DisplayName = "CheckState"),
	ResetState = 5 UMETA(DisplayName = "ResetState"),
	SweepState = 6 UMETA(DisplayName = "SweepState")
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
	void AnalyseState();
	void EnablePinsPhysics();

	// Variables
	TArray<AActor*> BowlingPins;
	bool PinsBeingChecked = false;

public:

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	BowlingState BowlingState = BowlingState::NewGame;

	void ChangeState(uint8 BowlingStateIndex);

	// State events

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
