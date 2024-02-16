// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//xSize = 150;
	//ySize = 150;
	//zSize = 150;

	tileSize = 1200;
	tileHeight = 480;
	//numSeeds = 40;

	levelGraph = new VoronoiGraph(horizontalLoad, verticalLoad, 1000);
}

//Use given graph to spawn rooms in the world
void ALevelGenerator::SpawnTilesFromGraph(VoronoiGraph graph, Point smallCorner)
{
	//UE_LOG(LogTemp, Warning, TEXT("SPAWNING"));
	for (int i = smallCorner.x; i < smallCorner.x + horizontalLoad; i++)
	{
		for (int k = smallCorner.y; k < smallCorner.y + horizontalLoad; k++)
		{
			for (int j = smallCorner.z; j < smallCorner.z + verticalLoad; j++)
			{
				if (graph.GetEdgesPoint(Point(i, k, j)) > 1)
				{
					FVector spawnLocation = GetTransform().GetLocation();
					FVector spawnOffset = FVector(i * tileSize, k * tileSize, j * tileHeight);

					spawnLocation += spawnOffset;

					ATestTile* tileRef = GetWorld()->SpawnActor<ATestTile>(BP_testTile, FTransform(spawnLocation));
				}
			}
		}
	}
}

/*Print to console for debugging
void ALevelGenerator::PrintGraphToConsole(VoronoiGraph graph)
{
	for (int i = 0; i < xSize; i++)
	{
		std::string printString = "";

		for (int k = 0; k < ySize; k++)
		{
			printString.append(std::to_string(graph.GetLevelPoint(i, k, zSize-1)));
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(printString.c_str()));
	}
}
*/


// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	//SpawnTestTile();

	//UE_LOG(LogTemp, Warning, TEXT("STARTING THE CREATION PROCESS"));

	levelGraph->FillLevelGraph(5, Point(0, 0, 0));

	//UE_LOG(LogTemp, Warning, TEXT("FINISHED CREATION, STARTING TRACE"));
	//PrintGraphToConsole(levelGraph);
	levelGraph->ConvolutionalTrace(Point(0, 0, 0));

	SpawnTilesFromGraph(*levelGraph, Point(0, 0, 0));

}

void ALevelGenerator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	timeFrame += deltaTime;

	if (((int)timeFrame) % 9 == 1)
	{
		levelGraph->FillLevelGraph(5, Point(20 * numGenerated, 0, 0));
	}
	else if (((int)timeFrame) % 9 == 3)
	{
		levelGraph->ConvolutionalTrace(Point(20 * numGenerated, 0, 0));
	}
	else if (((int)timeFrame) % 9 == 5)
	{
		SpawnTilesFromGraph(*levelGraph, Point(20 * numGenerated, 0, 0));
		numGenerated++;
	}
}
