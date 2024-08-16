// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\Button.h"
#include "MainMenu.generated.h"

UENUM(BlueprintType)
enum class CursorDirection : uint8
{
	Up = 0 UMETA(DisplayName = "Up"),
	Down = 1 UMETA(DisplayName = "Down"),
	Left = 2 UMETA(DisplayName = "Left"),
	Right = 3 UMETA(DisplayName = "Right")
};

/**
 *
 */
UCLASS()
class BOWLING_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	// Functions

	int32 GetCorrectIndexForButtonToHighlight(CursorDirection CursorDirection);

	// Variables

	int32 HighlightedButtonIndex = 0;

	class ABowlingGameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> OnePlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> TwoPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ThreePlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> FourPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> FivePlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SixPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SevenPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> EightPlayers;

	TArray<TObjectPtr<UButton>> NumberOfPlayersButtons;


public:

	// Functions
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void MoveCursor(int32 DirectionIndex);

	UFUNCTION(BlueprintCallable)
	void HighlightNumberOfPlayersButton(int32 Index);


	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfPlayers = 1;
};
