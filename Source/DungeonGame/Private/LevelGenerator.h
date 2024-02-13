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

	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int xSize;
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int ySize;
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int zSize;

	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int tileSize;
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int tileHeight;
	
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings")
	int numSeeds;

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
	private:
		int xSize, ySize, zSize;
		int numSeeds;

		int* graph = new int[xSize * ySize * zSize];

	public:
		
		struct Point *seedsList = new struct Point[numSeeds];

		void SetPoint(struct Point p, int val)
		{
			graph[p.x 
				+ p.y * ySize 
				+ p.z * ySize * zSize] = val;
		}

		int GetPoint(struct Point p)
		{
			return graph[p.x
				+ p.y * ySize
				+ p.z * ySize * zSize];
		}
		int GetPoint(int x, int y, int z)
		{
			return graph[x
				+ y * ySize
				+ z * ySize * zSize];
		}

		VoronoiGraph(int xSize, int ySize, int zSize, int numSeeds)
		{
			this->xSize = xSize;
			this->ySize = ySize;
			this->zSize = zSize;
			this->numSeeds = numSeeds;

			for (int i = 0; i < xSize * ySize * zSize; i++)
			{
				graph[i] = 0;
			}
		}

		~VoronoiGraph()
		{
			//delete[] graph;
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

	void TraceEdges_BAD(VoronoiGraph* graph);

	VoronoiGraph ConvolutionalTrace(VoronoiGraph graph);

	//Use given graph to spawn tiles in the world
	void SpawnTilesFromGraph(VoronoiGraph graph);

	void PrintGraphToConsole(VoronoiGraph graph);

	int CalcManhattanDistance(struct Point p1, struct Point p2);

	int CalcPythagoreanDistance(struct Point p1, struct Point p2);

	void EndGeneration(VoronoiGraph* graph);

	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
