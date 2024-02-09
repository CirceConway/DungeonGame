// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

const int xSize = 100;
const int ySize = 100;
//const int z_size = 100;

struct voronoiGraph {
	int graph[xSize][ySize];
};

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
voronoiGraph CreateVoronoi(int points)
{
	voronoiGraph voronoi;

	for (int i = 0; i < points; i++)
	{
		bool success = false;

		while (!success)
		{
			int randX = rand() % xSize;
			int randY = rand() % ySize;
			if (voronoi.graph[randX][randY] == 0)
			{
				voronoi.graph[randX][randY] = 1;
				success = true;
			}
		}

		
	}

	return voronoi;
}

//Use given graph to spawn rooms in the world
void SpawnTilesFromGraph(int &graph)
{
	;
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


