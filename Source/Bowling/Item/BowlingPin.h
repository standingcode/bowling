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

public:
	int GetPinNumber();
	bool IsStanding();
	FVector GetFVector();
};
