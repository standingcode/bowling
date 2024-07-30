// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringWidget.h"

//// Make constructor for the ScoringWidget class
//UScoringWidget::UScoringWidget(const FObjectInitializer& Object) :Super(Object)
//{
//	// Set the default values for the properties
//	if (PlayerNameText != nullptr)
//	{
//		PlayerNameText->SetText(FText::FromString(TEXT("By golly it works!")));
//	}
//}

void UScoringWidget::SetNameText(FString text)
{
	// Set the default values for the properties
	if (PlayerNameText != nullptr)
	{
		PlayerNameText->SetText(FText::FromString(text));
	}
}

void UScoringWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the PlayerNameText
	PlayerNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNameText")));

	bowl_score_0 = Cast<UTextBlock>(GetWidgetFromName(TEXT("bowl_score_0")));

	bowl_score_0->SetText(FText::FromString(TEXT("99")));
}
