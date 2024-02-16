// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TilesList.h"
#include "Tiles/TestTile.h"
#include "VoronoiGraph.h"
#include "LevelGenerator.generated.h"

UCLASS()
class ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<ATestTile> BP_testTile;

	//UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	const static int horizontalLoad = 10;
	//UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	const static int verticalLoad = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int tileSize;
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int tileHeight;
	
	//UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	const static int numSeeds = 100;

	float timeFrame = 0;

	int numGenerated = 1;


	// Sets default values for this actor's properties
	ALevelGenerator();

protected:

	VoronoiGraph* levelGraph;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Use given graph to spawn tiles in the world
	void SpawnTilesFromGraph(VoronoiGraph graph, Point smallCorner);

	void PrintGraphToConsole(VoronoiGraph graph);

	void Tick(float deltaTime) override;
};
