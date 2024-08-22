
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* Audio;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SidewaysSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BowlingForce = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveLeftAndRightLimit = 72.0f;

	bool BallIsInMotion = false;
	bool BallWasInMotion = false;

	UPROPERTY()
	class ABowlingGameModeBase* GameMode;

	UFUNCTION(BlueprintNativeEvent)
	void PlayerBowled();


	// TODO: Dev stuff maybe delete later
	void DevKnockPinsDown(int32 NumberOfPins);

	template<int32 NumberOfPins>
	void DevKnockPinsDown()
	{
		DevKnockPinsDown(NumberOfPins);
	}

private:

	// Variables
	UPROPERTY()
	FVector InitialBallPosition;

	// Functions
	UFUNCTION()
	void MoveLeftAndRight(float Value);

	UFUNCTION()
	void Bowl();

	UFUNCTION()
	void Quit();

	UFUNCTION(BlueprintCallable)
	void CheckCurrentBallSpeed(FVector velocity);

	UFUNCTION(BlueprintCallable)
	void CheckCurrentBallVerticalPositionEndBowlIfBallDroppedOffEdge(float ZPosition);

	UFUNCTION()
	void DisableCollisions();

	UFUNCTION()
	void EnableCollisions();

	UFUNCTION()
	void ReportBallOffEdgeOrStoppedMoving();

	UFUNCTION()
	void ResetBallPosition();

	UFUNCTION()
	void HideBall();

	UFUNCTION()
	void ShowBall();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ResetBall();
};
