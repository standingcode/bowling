// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringWidget.h"


void UScoringWidget::SetNameText(FString text)
{
	if (PlayerNameText != nullptr)
	{
		PlayerNameText->SetText(FText::FromString(text));
	}
}

void UScoringWidget::SetFrameScore(int32 FrameIndex, int32 Bowl1, int32 Bowl2, int32 Bowl3, int32 FrameTotal)
{
	if (!BowlScores.IsValidIndex(FrameIndex)) { return; }

	if (Bowl1 == 10)
	{
		// If we're in the last frame
		if (FrameIndex == 9)
		{
			BowlScores[(FrameIndex * 2)]->SetText(FText::FromString("X"));
		}
		else
		{
			BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("X"));

			if (FrameTotal != -1)
			{
				FrameTotals[FrameIndex]->SetText(FText::FromString(FString::FromInt(FrameTotal)));
			}

			return;
		}
	}
	else if (Bowl1 == 0)
	{
		BowlScores[FrameIndex * 2]->SetText(FText::FromString("-"));
	}
	else if (Bowl1 != -1)
	{
		BowlScores[FrameIndex * 2]->SetText(FText::FromString(FString::FromInt(Bowl1)));
	}

	if (Bowl2 == 10)
	{
		// We're in the last frame
		if (FrameIndex == 9)
		{
			BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("X"));
		}
		else
		{
			BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("/"));
		}
	}
	else if (Bowl2 == 0)
	{
		BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("-"));
	}
	else if (Bowl2 != -1)
	{
		if (Bowl1 + Bowl2 == 10)
		{
			BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString("/"));
		}
		else
		{
			BowlScores[(FrameIndex * 2) + 1]->SetText(FText::FromString(FString::FromInt(Bowl2)));
		}
	}

	if (Bowl3 == 10)
	{
		BowlScores[(FrameIndex * 2) + 2]->SetText(FText::FromString("X"));
	}
	else if (Bowl3 == 0)
	{
		BowlScores[(FrameIndex * 2) + 2]->SetText(FText::FromString("-"));
	}
	else if (Bowl3 != -1)
	{
		BowlScores[(FrameIndex * 2) + 2]->SetText(FText::FromString(FString::FromInt(Bowl3)));
	}

	if (FrameTotal != -1)
	{
		FrameTotals[FrameIndex]->SetText(FText::FromString(FString::FromInt(FrameTotal)));
	}

	if (FrameIndex == 9)
	{
		if (FrameTotal != -1)
		{
			FinalScore->SetText(FText::FromString(FString::FromInt(FrameTotal)));
		}
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

	for (int32 i = 0; i <= 21; i++)
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

	TArray<BowlingFrameScore*>* FrameScores = BowlingPLayer->GetAllFrameScores();

	for (int32 i = 0; i < FrameScores->Num(); i++)
	{
		BowlingFrameScore* FrameScore = (*FrameScores)[i];

		if (FrameScore->FirstBowl == -1) { break; }

		SetFrameScore(i, FrameScore->FirstBowl, FrameScore->SecondBowl, FrameScore->ThirdBowl, FrameScore->TotalRunningScore);
	}
}
