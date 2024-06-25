// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

// Sets default values
ABowlingPin::ABowlingPin()
{

}

int ABowlingPin::GetPinNumber()
{
	return PinNumber;
}

bool ABowlingPin::IsStanding()
{
	// The pin might be falling through the universe, if so it is not standing, return false
	if (PinFellOffEdge()) { return false; }

	// Is the pin standing up?
	return GetActorUpVector().Z > MinimumValueToConsiderStillStanding;
}

bool ABowlingPin::PinFellOffEdge()
{
	return GetActorLocation().Z < 0.0f;
}
