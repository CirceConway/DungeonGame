// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"



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
	xSize = 150;
	ySize = 150;
	zSize = 150;

	tileSize = 1200;
	tileHeight = 480;
	numSeeds = 40;


}

int ALevelGenerator::CalcManhattanDistance(struct Point p1, struct Point p2)
{
	int distX = abs(p1.x - p2.x);
	int distY = abs(p1.y - p2.y);
	int distZ = abs(p1.z - p2.z);

	return distX + distY + distZ;
}

int ALevelGenerator::CalcPythagoreanDistance(struct Point p1, struct Point p2)
{
	int distX = (p1.x - p2.x)^2;
	int distY = (p1.y - p2.y)^2;
	int distZ = (p1.z - p2.z)^2;

	return (int)sqrt(distX + distY + distZ);
}


void ALevelGenerator::EndGeneration(VoronoiGraph* graph)
{
	delete graph;
}

//Creates paths of the floor, returns int array of [size * size]
//Each index represents space for one potential room
ALevelGenerator::VoronoiGraph ALevelGenerator::CreateVoronoi()
{
	VoronoiGraph voronoi(xSize, ySize, zSize, numSeeds);

	//Place the seeds on the voronoi graph
	for (int i = 0; i < numSeeds; i++)
	{
		
		bool success = false;

		while (!success)
		{
			struct Point point(rand() % xSize, rand() % ySize, rand() % zSize);

			if (voronoi.GetPoint(point) != -1)
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
				for (int j = 0; j < numSeeds; j++)
				{
					//int dist = CalcPythagoreanDistance(p1, voronoi.seedsList[j]);
					int dist = CalcManhattanDistance(p1, voronoi.seedsList[j]);

					if (dist < shortestDist)
					{
						shortestDist = dist;
						shortestIndex = j;
					}
				}
				voronoi.SetPoint(p1, shortestIndex + 2);
				/*
				if (voronoi.graph[p1.x][p1.y][p1.z] != -1)
				{
					
					//UE_LOG(LogTemp, Warning, TEXT("Point (%d, %d) is %d"), p1.x, p1.y, voronoi.graph[p1.x][p1.y]);
				}
				*/
			}
				
		}
	}
	return voronoi;
}

//Scans left to right, marking the locations where numbers change with a 1, and everything else with a 0
//All -1s become 0s
void ALevelGenerator::TraceEdges_BAD(VoronoiGraph* graph)
{
	for (int i = 0; i < xSize - 1; i++)
	{
		for (int k = 0; k < ySize - 1; k++)
		{
			for (int j = 0; j < zSize - 1; j++)
			{
				struct Point p(i, k, j);
				/*
				if (((graph->GetPoint(p) == graph->GetPoint(i + 1k,j))
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
				*/
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

//QUINTUPLE NESTED FOR LOOP BABBEEEEEEEEEEEEE
//Dear future Circe, I'm so sorry
ALevelGenerator::VoronoiGraph ALevelGenerator::ConvolutionalTrace(VoronoiGraph graph)
{
	VoronoiGraph edges(xSize, ySize, zSize, numSeeds);
	int neighbors[6] = { 0, 0, 0, 0, 0, 0 };
	int regions;
	//Iterate through the graph
	for (int i = 1; i < xSize - 1; i++)
	{
		for (int k = 1; k < ySize - 1; k++)
		{
			for (int j = 1; j < zSize - 1; j++)
			{
				regions = 0;

				neighbors[0] = graph.GetPoint(i - 1, k, j);
				neighbors[1] = graph.GetPoint(i + 1, k, j);
				neighbors[2] = graph.GetPoint(i, k - 1, j);
				neighbors[3] = graph.GetPoint(i, k + 1, j);
				neighbors[4] = graph.GetPoint(i, k, j - 1);
				neighbors[5] = graph.GetPoint(i, k, j + 1);

				//Count how many unique regions are next to the point
				for (int x = 1; x < 6; x++)
				{
					int y;
					for (y = 0; y < x; y++)
					{
						if (neighbors[x] == neighbors[y])
							break;
					}
					if (x == y)
						regions++;
				}

				edges.SetPoint(Point(i, j, k), regions);


			}
		}
	}

	return edges;
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
				if (graph.GetPoint(i, k, j) > 1)
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
			//printString.append(std::to_string(graph.graph[i][k][zSize-1]));
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(printString.c_str()));
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
	VoronoiGraph edges = ConvolutionalTrace(myVoronoi);
	//EndGeneration(&myVoronoi);
	//PrintGraphToConsole(myVoronoi);
	SpawnTilesFromGraph(edges);
	//EndGeneration(&edges);

}


