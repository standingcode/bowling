// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	class ABowlingGameModeBase* GameMode;


public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfPlayers = 1;
};
