// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingItemBase.h"
#include "Kismet\GameplayStatics.h"
#include "BowlingPin.h"

// Sets default values
ABowlingItemBase::ABowlingItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components - Before this step they won't exist on our Actor, they've only been defined in the header
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ABowlingItemBase::BeginPlay()
{
	Super::BeginPlay();
}
