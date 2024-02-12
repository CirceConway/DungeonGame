// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TilesList.h"
#include "Tiles/TestTile.h"
#include "LevelGenerator.generated.h"

UCLASS()
class ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<ATestTile> BP_testTile;

	UFUNCTION()
	void SpawnTestTile();

	
	const static int xSize = 50;
	const static int ySize = 50;
	const static int zSize = 50;

	const static int tileSize = 1200;
	const static int tileHeight = 480;
	
	const static int numPoints = 15;

	struct Point {
		int x, y, z;

		Point(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Point()
		{
			x = 0;
			y = 0;
			z = 0;
		}

	};

	class VoronoiGraph {
	public:
		int graph[xSize][ySize][zSize];
		struct Point seedsList[numPoints];

		void SetPoint(struct Point p, int val)
		{
			graph[p.x][p.y][p.z] = val;
		}

		VoronoiGraph()
		{
			for (int i = 0; i < xSize; i++)
			{
				for (int k = 0; k < ySize; k++)
				{
					for (int j = 0; j < zSize; j++)
					{
						graph[i][k][j] = 0;
					}
					
				}
			}
		}
	};

	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Creates paths of the floor, returns int array of [size * size]
	//Each index represents space for one potential room
	VoronoiGraph CreateVoronoi();

	void TraceEdges(VoronoiGraph* graph);

	//Use given graph to spawn tiles in the world
	void SpawnTilesFromGraph(VoronoiGraph graph);

	void PrintGraphToConsole(VoronoiGraph graph);

	int CalcManhattanDistance(struct Point p1, struct Point p2);

	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
