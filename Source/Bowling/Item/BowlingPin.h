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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PinNumber = -1;

	float MinimumValueToConsiderStillStanding = 0.97f;
	FVector OriginalLocation;
	FRotator OriginalRotation;
	bool PinFellOffEdge = false;
	float RootOriginalLocationZ;

	// Functions

	ABowlingPin();
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	void HidePin();
	void ShowPin();
	void CheckIfPinFellOffEdge();
	float GetRootZOffsetComparedToOriginalLocation();

public:

	// Functions
	void ResetToOriginalPositionAndRotation();

	// Variables
	int GetPinNumber();
	bool IsStanding();
	bool DidFallOffEdge();
	void ResetPin();
	void EnableCollisionsAndPhysics();
	void DisableCollisionsAndPhysics();
	AActor* Parent;
};
