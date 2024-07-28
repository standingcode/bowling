
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BowlingBall.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BOWLING_API ABowlingBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABowlingBall();

protected:

	// Functions
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	// Definite components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent* Mesh;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SidewaysSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BowlingForce = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MoveLeftAndRightLimit = 72.0f;

	bool BallIsInMotion = false;
	bool BallWasInMotion = false;

	class ABowlingGameModeBase* GameMode;

private:

	// Variables
	FVector InitialBallPosition;

	// Functions
	void MoveLeftAndRight(float Value);
	void Bowl();

	UFUNCTION(BlueprintCallable)
	void CheckCurrentBallSpeed(FVector velocity);

	UFUNCTION(BlueprintCallable)
	void CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(float ZPosition);

	void DisableCollisions();
	void EnableCollisions();

	void ReportBallOffEdgeOrStoppedMoving();

	void ResetBallPosition();
	void HideBall();
	void ShowBall();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ResetBall();
};
