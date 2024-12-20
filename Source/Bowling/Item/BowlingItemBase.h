// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BowlingItemBase.generated.h"

UCLASS()
class BOWLING_API ABowlingItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABowlingItemBase();

	// Define components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


};
