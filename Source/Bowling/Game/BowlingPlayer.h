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
	int32 Score;
	TArray<class BowlingFrameScore*> FrameScores;

public:
	BowlingPlayer(FString Name);
	~BowlingPlayer();

	// Functions
	void SetName(FString Name);
	int32 GetRunningScoreForFinalFrame();
	FString GetName();
	TArray<class BowlingFrameScore*>* GetAllFrameScores();
};

class BOWLING_API BowlingFrameScore
{
public:
	int32 FirstBowl = -1;
	int32 SecondBowl = -1;
	int32 ThirdBowl = -1;

	int32 TotalRunningScore = -1;
};