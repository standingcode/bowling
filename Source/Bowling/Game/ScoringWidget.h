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
	void SetBowlScore(int32 BowlIndex, int32 Score);
	void SetRoundTotal(int32 BowlIndex, int32 Score);
	void SetFinalScore(int32 Score);

	// Some kind of set round score method?

	// WIDGET REFERENCES

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	// Final score
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> FinalScore;

	// Round totals
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UTextBlock>> RoundTotals;

	// Bowl scores

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UTextBlock>> BowlScores;

public:

	virtual void NativeConstruct() override;

	void SetScorecardData(BowlingPlayer* BowlingPLayer);
};
