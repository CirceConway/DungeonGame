// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"

const static int xSize = 50;
const static int ySize = 50;
const static int zSize = 50;

const static int numPoints = 6;

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

int ALevelGenerator::CalcManhattanDistance(struct Point p1, struct Point p2)
{
	int distX = abs(p1.x - p2.x);
	int distY = abs(p1.y - p2.y);

	return distX + distY;
}

//Creates paths of the floor, returns int array of [size * size]
//Each index represents space for one potential room
ALevelGenerator::VoronoiGraph ALevelGenerator::CreateVoronoi()
{
	VoronoiGraph voronoi;

	//Place the seeds on the voronoi graph
	for (int i = 0; i < numPoints; i++)
	{
		
		bool success = false;

		while (!success)
		{
			struct Point point(rand() % xSize, rand() % ySize);

			if (voronoi.graph[point.x][point.y] != 9)
			{
				voronoi.SetPoint(point, 9);
				voronoi.pointList[i] = point;

				success = true;
			}
		}		
	}

	for (int i = 0; i < xSize; i++)
	{
		for (int k = 0; k < ySize; k++)
		{
			int shortestDist = xSize + ySize + zSize + 1;
			int shortestIndex = 0;
			struct Point p1(i, k);

			for (int j = 0; j < numPoints; j++)
			{
				int dist = CalcManhattanDistance(p1, voronoi.pointList[j]);

				if (dist < shortestDist)
				{
					shortestDist = dist;
					shortestIndex = j;
				}
			}
			if (voronoi.graph[p1.x][p1.y] != 9)
			{
				voronoi.SetPoint(p1, shortestIndex);
			}
			
		}
	}

	return voronoi;
}

//Use given graph to spawn rooms in the world
void ALevelGenerator::SpawnTilesFromGraph(VoronoiGraph graph)
{
	for (int i = 0; i < xSize; i++)
	{
		std::string printString = "";
		for (int k = 0; k < ySize; k++)
		{
			printString.append(std::to_string(graph.graph[i][k]));
			
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(printString.c_str()));
	}
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("STARTING THE CREATION PROCESS"));
	VoronoiGraph myVoronoi = CreateVoronoi();
	UE_LOG(LogTemp, Warning, TEXT("FINISHED CREATION"));
	SpawnTilesFromGraph(myVoronoi);

}

/* Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/


