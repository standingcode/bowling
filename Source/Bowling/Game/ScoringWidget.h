// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\TextBlock.h"
#include "BowlingWidget.h"
#include "ScoringWidget.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API UScoringWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	//UScoringWidget(const FObjectInitializer& Object);

	void SetNameText(FString text);

	virtual void NativeConstruct() override;
};
