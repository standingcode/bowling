// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class BOWLING_API ABowlingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	TArray<AActor*> BowlingPins;


public:

	void BowlFinished();

};
