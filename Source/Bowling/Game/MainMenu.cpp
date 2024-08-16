// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "BowlingGameModeBase.h"

void UMainMenu::HighlightNumberOfPlayersButton(int32 Index)
{
	FString TheIndex;
	TheIndex.AppendInt(Index);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TheIndex);

	for (int32 i = 0; i < NumberOfPlayersButtons.Num(); i++)
	{
		FButtonStyle ButtonStyle = NumberOfPlayersButtons[i].Get()->WidgetStyle;
		ButtonStyle.Normal.OutlineSettings.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
		ButtonStyle.Normal.OutlineSettings.Width = 0.0f;
		NumberOfPlayersButtons[i]->SetStyle(ButtonStyle);
	}

	NumberOfPlayers = Index + 1;

	FButtonStyle ButtonStyle = NumberOfPlayersButtons[Index].Get()->WidgetStyle;
	ButtonStyle.Normal.OutlineSettings.Color = FLinearColor::Green;
	ButtonStyle.Normal.OutlineSettings.Width = 10.0f;
	NumberOfPlayersButtons[Index]->SetStyle(ButtonStyle);
}

int32 UMainMenu::GetCorrectIndexForButtonToHighlight(CursorDirection CursorDirection)
{
	int32 NewIndex = -1;

	switch (CursorDirection)
	{
	case CursorDirection::Up:

		NewIndex = HighlightedButtonIndex - 4;

		return NewIndex >= 0 ? NewIndex : HighlightedButtonIndex;

		break;

	case CursorDirection::Down:

		NewIndex = HighlightedButtonIndex + 4;

		return NewIndex < NumberOfPlayersButtons.Num() ? NewIndex : HighlightedButtonIndex;

		break;

	case CursorDirection::Left:

		NewIndex = HighlightedButtonIndex - 1;

		return NewIndex >= 0 ? NewIndex : HighlightedButtonIndex;

		break;

	case CursorDirection::Right:

		NewIndex = HighlightedButtonIndex + 1;

		return NewIndex < NumberOfPlayersButtons.Num() ? NewIndex : HighlightedButtonIndex;

		break;

	default:

		return 0;

		break;
	}
}

void UMainMenu::NativeConstruct()
{
	NumberOfPlayersButtons.Add(OnePlayer);
	NumberOfPlayersButtons.Add(TwoPlayers);
	NumberOfPlayersButtons.Add(ThreePlayers);
	NumberOfPlayersButtons.Add(FourPlayers);
	NumberOfPlayersButtons.Add(FivePlayers);
	NumberOfPlayersButtons.Add(SixPlayers);
	NumberOfPlayersButtons.Add(SevenPlayers);
	NumberOfPlayersButtons.Add(EightPlayers);

	HighlightNumberOfPlayersButton(HighlightedButtonIndex);

	GameMode = Cast<ABowlingGameModeBase>(GetWorld()->GetAuthGameMode());
}

void UMainMenu::StartGame()
{
	GameMode->ChangeState(static_cast<uint8>(BowlingState::NewGame));
}

void UMainMenu::MoveCursor(int32 DirectionIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Direction);

	if (GameMode->IsGameInProgress() == true) { return; }

	enum CursorDirection CursorDirection = static_cast<enum CursorDirection>(DirectionIndex);

	HighlightedButtonIndex = GetCorrectIndexForButtonToHighlight(CursorDirection);
	HighlightNumberOfPlayersButton(HighlightedButtonIndex);
}
