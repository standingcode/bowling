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

	UPROPERTY()
	FVector OriginalLocation;

	UPROPERTY()
	FRotator OriginalRotation;

	bool PinFellOffEdge = false;
	float RootOriginalLocationZ;

	UPROPERTY()
	USceneComponent* OriginalParent;

	// Functions

	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	UFUNCTION()
	void HidePin();

	UFUNCTION()
	void ShowPin();

	UFUNCTION()
	void CheckIfPinFellOffEdge();

	UFUNCTION()
	float GetRootZOffsetComparedToOriginalLocation();

public:

	// Functions
	UFUNCTION()
	void ResetToOriginalPositionAndRotation();

	// Variables

	int GetPinNumber();
	bool IsStanding();
	bool DidFallOffEdge();
	void ResetPinToOriginalPositionAndHide();
	void ReadyPinForNewRound();
	void EnableCollisionsAndPhysics();
	void DisableCollisionsAndPhysics();
	void DetachFromParent();

	// TODO: Dev stuff maybe delete later
	void KnockPinDown();
};
