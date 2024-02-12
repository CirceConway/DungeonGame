// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"

//const static int xSize = 50;
//const static int ySize = 50;
//const static int zSize = 50;

//const static int numPoints = 8;



void ALevelGenerator::SpawnTestTile()
{
	//FActorSpawnParameters SpawnParams;

	UE_LOG(LogTemp, Warning, TEXT("SPAWNING TEST TILE"));

	if (BP_testTile)
	{
		ATestTile* tileRef = GetWorld()->SpawnActor<ATestTile>(BP_testTile, GetTransform());
	}
}

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
	int distZ = abs(p1.z - p2.z);

	return distX + distY + distZ;
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
			struct Point point(rand() % xSize, rand() % ySize, rand() % zSize);

			if (voronoi.graph[point.x][point.y][point.z] != -1)
			{
				voronoi.SetPoint(point, -1);
				voronoi.seedsList[i] = point;

				//UE_LOG(LogTemp, Warning, TEXT("Placed Seed %d at (%d, %d)"), i, point.x, point.y);

				success = true;
			}
		}		
	}

	for (int i = 0; i < xSize; i++)
	{
		for (int k = 0; k < ySize; k++)
		{
			for (int z = 0; z < zSize; z++)
			{
				int shortestDist = xSize + ySize + zSize + 1;
				int shortestIndex = 0;
				struct Point p1(i, k, z);
				for (int j = 0; j < numPoints; j++)
				{
					int dist = CalcManhattanDistance(p1, voronoi.seedsList[j]);

					if (dist < shortestDist)
					{
						shortestDist = dist;
						shortestIndex = j;
					}
				}
				if (voronoi.graph[p1.x][p1.y][p1.z] != -1)
				{
					voronoi.SetPoint(p1, shortestIndex + 2);
					//UE_LOG(LogTemp, Warning, TEXT("Point (%d, %d) is %d"), p1.x, p1.y, voronoi.graph[p1.x][p1.y]);
				}
			}
				
		}
	}
	return voronoi;
}

//Scans left to right, marking the locations where numbers change with a 1, and everything else with a 0
//All -1s become 0s
void ALevelGenerator::TraceEdges(VoronoiGraph* graph)
{
	for (int i = 0; i < xSize - 1; i++)
	{
		for (int k = 0; k < ySize - 1; k++)
		{
			for (int j = 0; j < zSize - 1; j++)
			{
				struct Point p(i, k, j);
				if (((graph->graph[i][k][j] == graph->graph[i][k + 1][j])
					&& (graph->graph[i][k][j] == graph->graph[i + 1][k][j])
					&& (graph->graph[i][k][j] == graph->graph[i][k][j + 1]))
					|| (graph->graph[i + 1][k][j] == -1)
					|| (graph->graph[i][k + 1][j] == -1)
					|| (graph->graph[i][k][j + 1] == -1))
				{
					graph->SetPoint(p, 0);
				}
				else if (graph->graph[i][k][j] == -1)
				{
					graph->SetPoint(p, 0);
				}
				else
				{
					graph->SetPoint(p, 1);
				}
			}
			//The end of this sets the last x, y and z coords to 0 instead of actually testing them
			//TODO: change when not feeling lazy
			graph->SetPoint(Point(i, k, zSize - 1), 0);
		}
		graph->SetPoint(Point(i, ySize - 1, zSize - 1), 0);
		/*
		struct Point p(i, ySize - 1);
		if (graph->graph[i][ySize - 1] == graph->graph[i + 1][ySize - 1])
		{
			graph->SetPoint(p, 0);
		}
		else
		{
			graph->SetPoint(p, 1);
		}
		*/
	}
	graph->SetPoint(Point(zSize - 1, ySize - 1, zSize - 1), 0);
	/*
	for (int k = 0; k < ySize - 1; k++)
	{
		struct Point p(xSize - 1, k);
		if ((graph->graph[xSize - 1][k] == graph->graph[xSize - 1][k + 1]) && (graph->graph[xSize - 1][k] != -1))
		{
			graph->SetPoint(p, 0);
		}
		else
		{
			graph->SetPoint(p, 1);
		}
		
	}
	*/
}

//Use given graph to spawn rooms in the world
void ALevelGenerator::SpawnTilesFromGraph(VoronoiGraph graph)
{
	for (int i = 0; i < xSize; i++)
	{
		for (int k = 0; k < ySize; k++)
		{
			for (int j = 0; j < zSize; j++)
			{
				if (graph.graph[i][k][j] == 1)
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

//Print to console for debugging
void ALevelGenerator::PrintGraphToConsole(VoronoiGraph graph)
{
	for (int i = 0; i < xSize; i++)
	{
		std::string printString = "";

		for (int k = 0; k < ySize; k++)
		{
			printString.append(std::to_string(graph.graph[i][k][zSize-1]));
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(printString.c_str()));
	}
}


// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	//SpawnTestTile();

	//UE_LOG(LogTemp, Warning, TEXT("STARTING THE CREATION PROCESS"));
	VoronoiGraph myVoronoi = CreateVoronoi();
	//UE_LOG(LogTemp, Warning, TEXT("FINISHED CREATION, STARTING TRACE"));
	//PrintGraphToConsole(myVoronoi);
	TraceEdges(&myVoronoi);
	//PrintGraphToConsole(myVoronoi);
	SpawnTilesFromGraph(myVoronoi);

}


