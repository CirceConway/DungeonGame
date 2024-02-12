// Fill out your copyright notice in the Description page of Project Settings.


#include "Tiles/TestTile.h"

// Sets default values
ATestTile::ATestTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

