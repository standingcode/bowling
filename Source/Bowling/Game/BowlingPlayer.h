// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BowlingPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BOWLING_API ABowlingPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABowlingPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Definite components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") class UCameraComponent* Camera;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SidewaysSpeed = 1.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite) float JumpImpulse = 500.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxJumpCount = 1;

private:

	// Functions
	void MoveLeftAndRight(float Value);

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
