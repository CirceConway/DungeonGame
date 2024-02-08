// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

//Creates paths of the floor, returns int array of [size * size]
//Each index represents space for one potential room
void ALevelGenerator::CreateFloorGraph(int size)
{
	;
}

//Use given graph to spawn rooms in the world
void ALevelGenerator::SpawnTilesFromGraph(int &graph)
{
	;
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


