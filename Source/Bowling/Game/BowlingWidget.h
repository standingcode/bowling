// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\CanvasPanel.h"
#include "Components\VerticalBox.h"
#include "BowlingPlayer.h"
#include "BowlingWidget.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API UBowlingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")	TSubclassOf<class UUserWidget> ScorecardWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBoxForScorecardWidget;

	void ShowScorecards(TArray<BowlingPlayer*> players);
};
