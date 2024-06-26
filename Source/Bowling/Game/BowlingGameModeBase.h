// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingGameModeBase.generated.h"

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

	// Variables
	TArray<AActor*> BowlingPins;
	bool CheckForPinsToStopMoving = false;

public:
	bool CanBowl = true;

	void BallReportedStoppedOrOffTheEdge();
	void Reset();

	UFUNCTION(BlueprintNativeEvent)
	void BowlFinished();
};
