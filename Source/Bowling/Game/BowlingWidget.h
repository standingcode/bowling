// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\CanvasPanel.h"
#include "Components\VerticalBox.h"
#include "BowlingPlayer.h"
#include "ScoringWidget.h"
#include "BowlingWidget.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API UBowlingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBoxForScorecardWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GameOverCanvasPanel;

	UPROPERTY(BlueprintReadWrite)
	TArray<UScoringWidget*> ScoringWidgets;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateScoringWidget(int32 NumberOfScorecards);
public:

	void ShowScorecards(TArray<BowlingPlayer*> players);

	UFUNCTION()
	void ShowGameOverText();
};
