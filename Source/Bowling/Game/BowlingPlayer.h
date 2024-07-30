// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class BOWLING_API BowlingPlayer
{
private:
	FString PlayerName;

	int Score;
	//TArray<BowlingScore> FrameScores;

public:
	BowlingPlayer(FString Name);
	~BowlingPlayer();

	// Functions
	void SetName(FString Name);
	FString GetName();

};

//// First bowl should be -1 for a strike, 0 for a miss, 1-9 for the number of pins knocked down
//class BOWLING_API BowlingScore
//{
//	int FirstBowl = -2;
//	int SecondBowl = -2;
//	int ThirdBowl = -2;
//
//	bool WasAStrike = FirstBowl == 10;
//	bool WasASpare = FirstBowl + SecondBowl == 10;
//
//	int TotalScore = 0;
//};
