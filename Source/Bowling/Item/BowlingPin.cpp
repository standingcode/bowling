// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

int ABowlingPin::GetPinNumber()
{
	return PinNumber;
}

bool ABowlingPin::IsStanding()
{
	return GetActorUpVector().ZAxisVector == FVector(0, 0, 1);
}
