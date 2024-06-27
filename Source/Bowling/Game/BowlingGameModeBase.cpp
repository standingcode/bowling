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
	if (CanBowl || !CheckForPinsToStopMoving) { return; }

	CheckForPinsToStopMoving = false;

	for (int32 i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);

		if (BowlingPin->DidFallOffEdge())
		{
			continue;
		}

		if (!BowlingPin->GetVelocity().IsNearlyZero())
		{
			CheckForPinsToStopMoving = true;
			return;
		}
	}

	// Need to stop this triggering multiple times
	BowlFinished();
}

void ABowlingGameModeBase::BowlFinished_Implementation()
{
	ShowResultsOfBowl();
}

void ABowlingGameModeBase::BallReportedStoppedOrOffTheEdge()
{
	CheckForPinsToStopMoving = true;
}
