// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringWidget.h"


void UScoringWidget::SetNameText(FString text)
{
	if (PlayerNameText != nullptr)
	{
		PlayerNameText->SetText(FText::FromString(text));
	}
}

void UScoringWidget::SetFrameScore(int32 FrameIndex, int32 Bowl1, int32 Bowl2, int32 FrameTotal)
{
	if (!BowlScores.IsValidIndex(FrameIndex)) { return; }

	if (Bowl1 == 10)
	{
		BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("X"));

		if (FrameTotal != -1)
		{
			FrameTotals[FrameIndex]->SetText(FText::FromString(FString::FromInt(FrameTotal)));
		}

		return;
	}
	else if (Bowl1 == 0)
	{
		BowlScores[FrameIndex * 2]->SetText(FText::FromString("—"));
	}
	else
	{
		BowlScores[FrameIndex * 2]->SetText(FText::FromString(FString::FromInt(Bowl1)));
	}

	if (Bowl2 == 0)
	{
		BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("—"));
	}
	else
	{
		BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString(FString::FromInt(Bowl1)));
	}

	if (FrameTotal != -1)
	{
		FrameTotals[FrameIndex]->SetText(FText::FromString(FString::FromInt(FrameTotal)));
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
			FrameTotals.Add(TempTextBlock);
		}
	}
}

void UScoringWidget::SetScorecardData(BowlingPlayer* BowlingPLayer)
{
	SetNameText(BowlingPLayer->GetName());

	TArray<BowlingFrameScore*> FrameScores = BowlingPLayer->GetAllFrameScores();

	for (int32 i = 0; i < FrameScores.Num(); i++)
	{
		if (FrameScores[i] != nullptr)
		{
			BowlingFrameScore* FrameScore = FrameScores[i];

			if (FrameScore->FirstBowl == -1) { break; }

			SetFrameScore(i, FrameScore->FirstBowl, FrameScore->SecondBowl, FrameScore->TotalFrameScore);
		}
	}
}

// If previous frame was a strike and we've completed the 2nd bowl of this frame we can fill the total for previous
//if (FrameScores[i - 1]->WasAStrike && i != 0)
//{
//	SetFrameTotal(i - 1, 10 + FrameScore->FirstBowl + FrameScore->SecondBowl);
//}

//// If previous frame was a spare and we've completed the 1st bowl of this frame we can fill the total for previous
//if (FrameScores[i - 1]->WasASpare && i != 0)
//{
//	SetFrameTotal(i - 1, 10 + FrameScore->FirstBowl);
//}

//// If the 2 scores are less than 10 we can fill the total for this frame now
//if (FrameScore->FirstBowl + FrameScore->SecondBowl < 10)
//{
//	SetFrameTotal(i, FrameScore->FirstBowl + FrameScore->SecondBowl);
//}
