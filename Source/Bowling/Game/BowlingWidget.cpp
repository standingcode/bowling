// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingWidget.h"
#include "BowlingPlayer.h"
#include "ScoringWidget.h"
#include "Blueprint/WidgetTree.h"


void UBowlingWidget::CreateScoringWidget_Implementation(int32 NumberOfScorecards)
{
}

void UBowlingWidget::ShowScorecards(TArray<BowlingPlayer*> players)
{
	if (!IsValid(VerticalBoxForScorecardWidget)) { return; }

	while (VerticalBoxForScorecardWidget->GetChildrenCount() > 1)
	{
		VerticalBoxForScorecardWidget->RemoveChildAt(VerticalBoxForScorecardWidget->GetChildrenCount() - 1);
	}

	CreateScoringWidget(players.Num());

	for (int32 i = 0; i < players.Num(); i++)
	{
		VerticalBoxForScorecardWidget->AddChildToVerticalBox(ScoringWidgets[i]);
		ScoringWidgets[i]->SetScorecardData(players[i]);
	}
}

void UBowlingWidget::ShowGameOverText()
{
	GameOverCanvasPanel->SetVisibility(ESlateVisibility::Visible);
}
