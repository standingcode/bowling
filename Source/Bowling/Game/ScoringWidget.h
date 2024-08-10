// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\TextBlock.h"
#include "BowlingPlayer.h"
#include "ScoringWidget.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API UScoringWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	void SetNameText(FString text);
	void SetFrameScore(int32 FrameIndex, int32 Bowl1, int32 Bowl2, int32 Bowl3, int32 FrameTotal);
	void SetFinalScore(int32 Score);

	// WIDGET REFERENCES

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	// Final score
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> FinalScore;

	// Round totals
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UTextBlock>> FrameTotals;

	// Bowl scores

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UTextBlock>> BowlScores;

public:

	virtual void NativeConstruct() override;

	void SetScorecardData(BowlingPlayer* BowlingPLayer);
};
