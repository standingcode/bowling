// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BowlingItemBase.h"
#include "BowlingPin.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API ABowlingPin : public ABowlingItemBase
{
	GENERATED_BODY()

protected:

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int PinNumber = -1;

	float MinimumValueToConsiderStillStanding = 0.97f;
	FVector OriginalPosition;
	FRotator OriginalRotation;
	bool PinFellOffEdge = false;

	// Functions

	ABowlingPin();
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	void HidePin();
	void ShowPin();
	void DisableCollisions();
	void EnableCollisions();
	void CheckIfPinFellOffEdge();

public:

	// Functions
	void ResetToOriginalPositionAndRotation();

	// Variables
	int GetPinNumber();
	bool IsStanding();
	bool DidFallOffEdge();
	void ResetPin();
};
