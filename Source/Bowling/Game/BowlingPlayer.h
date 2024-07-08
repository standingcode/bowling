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
public:
	BowlingPlayer(FString Name);
	~BowlingPlayer();

	// Variables
	void SetName(FString Name);
	FString GetName();
};
