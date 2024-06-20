// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Bowling\Item\BowlingPin.h"

void ABowlingGameModeBase::BeginPlay()
{
	// Get all of the bowling pins into the array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowlingPin::StaticClass(), BowlingPins);

	//for (size_t i = 0; i < BowlingPins.Num(); i++)
	//{
	//	ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
	//	GEngine->AddOnScreenDebugMessage(i, 10.0f, FColor::White, FString::Printf(TEXT("Bowling pin: %i"), BowlingPin->GetPinNumber()));
	//}

}

void ABowlingGameModeBase::BowlFinished()
{
	for (size_t i = 0; i < BowlingPins.Num(); i++)
	{
		ABowlingPin* BowlingPin = Cast<ABowlingPin>(BowlingPins[i]);
		GEngine->AddOnScreenDebugMessage(i, 10.0f, FColor::White, FString::Printf(TEXT("Bowling pin is knocked down?: %i"), BowlingPin->GetPinNumber()));
	}
}
