// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

// Sets default values
ABowlingPin::ABowlingPin()
{
	//OriginalUpVector = GetActorUpVector();
}

int ABowlingPin::GetPinNumber()
{
	return PinNumber;
}

bool ABowlingPin::IsStanding()
{
	return GetActorUpVector().Z > MinimumValueToConsiderStillStanding;
}