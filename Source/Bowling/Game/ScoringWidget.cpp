// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringWidget.h"


void UScoringWidget::SetNameText(FString text)
{
	if (PlayerNameText != nullptr)
	{
		PlayerNameText->SetText(FText::FromString(text));
	}
}

void UScoringWidget::SetBowlScore(int32 BowlIndex, int32 Score)
{
	if (BowlScores.IsValidIndex(BowlIndex))
	{
		BowlScores[BowlIndex]->SetText(FText::FromString(FString::FromInt(Score)));
	}
}

void UScoringWidget::SetRoundTotal(int32 BowlIndex, int32 Score)
{
	if (RoundTotals.IsValidIndex(BowlIndex))
	{
		RoundTotals[BowlIndex]->SetText(FText::FromString(FString::FromInt(Score)));
	}
}

void UScoringWidget::SetFinalScore(int32 Score)
{
	if (FinalScore != nullptr)
	{
		FinalScore->SetText(FText::FromString(FString::FromInt(Score)));
	}
}

void UScoringWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i <= 20; i++)
	{
		UTextBlock* TempTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName("bowl_score_" + FString::FromInt(i))));
		if (TempTextBlock != nullptr)
		{
			BowlScores.Add(TempTextBlock);
		}
	}

	for (int32 i = 0; i <= 9; i++)
	{
		UTextBlock* TempTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName("round_total_" + FString::FromInt(i))));
		if (TempTextBlock != nullptr)
		{
			RoundTotals.Add(TempTextBlock);
		}
	}
}

void UScoringWidget::SetScorecardData(BowlingPlayer* BowlingPLayer)
{
	SetNameText(BowlingPLayer->GetName());
}
