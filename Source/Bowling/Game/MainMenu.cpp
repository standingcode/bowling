// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "BowlingGameModeBase.h"

void UMainMenu::NativeConstruct()
{
	GameMode = Cast<ABowlingGameModeBase>(GetWorld()->GetAuthGameMode());
}

void UMainMenu::StartGame()
{
	GameMode->ChangeState(static_cast<uint8>(BowlingState::NewGame));
}
