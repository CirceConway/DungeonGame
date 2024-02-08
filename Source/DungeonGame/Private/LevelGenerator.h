// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Creates paths of the floor, returns int array of [size * size]
	//Each index represents space for one potential room
	void CreateFloorGraph(int size);

	//Use given graph to spawn tiles in the world
	void SpawnTilesFromGraph(int &graph);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
