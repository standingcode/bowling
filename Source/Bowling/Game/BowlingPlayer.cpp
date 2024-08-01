// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPlayer.h"

BowlingPlayer::BowlingPlayer(FString Name)
{
	SetName(Name);

	FrameScores.Init(nullptr, 10);
}

BowlingPlayer::~BowlingPlayer()
{
}

void BowlingPlayer::SetName(FString Name)
{
	PlayerName = Name;
}

FString BowlingPlayer::GetName()
{
	return PlayerName;
}

TArray<class BowlingFrameScore*> BowlingPlayer::GetAllFrameScores()
{
	return FrameScores;
}
