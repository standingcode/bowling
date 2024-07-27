// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingWidget.h"
#include "BowlingPlayer.h"
#include "ScoringWidget.h"

void UBowlingWidget::ShowScorecards(TArray<BowlingPlayer*> players)
{
	if (VerticalBoxForScorecardWidget == nullptr) { return; }

	for (int32 i = 0; i < players.Num(); i++)
	{
		UScoringWidget* ScoringWidget = CreateWidget<UScoringWidget>(GetWorld(), ScorecardWidgetClass);
		//ScoringWidget->SetPlayer(players[i]);
		VerticalBoxForScorecardWidget->AddChild(ScoringWidget);
	}

}
