// Fill out your copyright notice in the Description page of Project Settings.

#include "VoronoiGraph.h"

VoronoiGraph::VoronoiGraph(int loadedHorizontal, int loadedVertical, int maxSeeds)
{
	//Add 2, because ConvolutionalTrace ignores the first and last layer of every dimension
	this->loadedHorizontal = loadedHorizontal + 2;
	this->loadedVertical = loadedVertical + 2;
	this->maxSeeds = maxSeeds;
	horizontalSquare = this->loadedHorizontal * this->loadedHorizontal;

	seedsList = new Point[maxSeeds];
	numSeeds = 0;

	levelGraph = new int[horizontalSquare * loadedVertical];
	edgesGraph = new int[horizontalSquare * loadedVertical];

	for (int i = 0; i < horizontalSquare * loadedVertical; i++)
	{
		levelGraph[i] = 0;
		edgesGraph[i] = 0;
	}

	//UE_LOG(LogTemp, Warning, TEXT("FINISHED CONSTRUCTOR"));
}

#pragma region Getters and Setters
//This function MUST be used any time the graphs are read/written after their creation
void VoronoiGraph::ModPoints(Point* p) const
{
	p->x = p->x % loadedHorizontal;
	p->y = p->y % loadedHorizontal;
	p->z = p->z % loadedVertical;
}

void VoronoiGraph::SetLevelPoint(Point p, int val)
{
	ModPoints(&p);

	levelGraph[p.x
		+ p.y * loadedHorizontal
		+ p.z * horizontalSquare] = val;
}

void VoronoiGraph::SetEdgesPoint(Point p, int val)
{
	ModPoints(&p);

	edgesGraph[p.x
		+ p.y * loadedHorizontal
		+ p.z * horizontalSquare] = val;
}

int VoronoiGraph::GetLevelPoint(Point p)
{
	ModPoints(&p);

	return levelGraph[p.x
		+ p.y * loadedHorizontal
		+ p.z * horizontalSquare];
}

int VoronoiGraph::GetLevelPoint(int x, int y, int z)
{
	Point p(x, y, z);
	return GetLevelPoint(p);
}

int VoronoiGraph::GetEdgesPoint(Point p)
{
	ModPoints(&p);

	return edgesGraph[p.x
		+ p.y * loadedHorizontal
		+ p.z * horizontalSquare];
}

//Adds each seed with an insertion sort, sorting ascending by x, y, z
int VoronoiGraph::AddSeed(Point p)
{
	int startSearch = 0;
	int endSearch = numSeeds - 1;

	int midpoint = (endSearch + startSearch) / 2;

	int dimension = 0;

	while (endSearch - startSearch > 0)
	{
		midpoint = (endSearch + startSearch) / 2;

		if (seedsList[midpoint].Val(dimension) > p.Val(dimension))
		{
			endSearch = midpoint - 1;
		}
		else if ((seedsList[midpoint].Val(dimension) < p.Val(dimension)))
		{
			startSearch = midpoint + 1;
		}
		else
		{
			if (dimension == 2)
			{
				return 0;
			}
			startSearch, endSearch = midpoint;
			while (seedsList[startSearch - 1].Val(dimension) == p.Val(dimension))
			{
				startSearch--;
			}
			while (seedsList[endSearch + 1].Val(dimension) == p.Val(dimension))
			{
				endSearch++;
			}
			dimension++;
		}
	}

	int indexToAdd = 0;
	if (endSearch < midpoint)
	{
		indexToAdd = endSearch;
	}
	else if (startSearch >= midpoint)
	{
		indexToAdd = startSearch;
	}

	for (int i = numSeeds; i > indexToAdd; i--)
	{
		seedsList[numSeeds] = seedsList[numSeeds - 1];
	}

	

	seedsList[indexToAdd] = p;
	numSeeds++;

	//UE_LOG(LogTemp, Warning, TEXT("Placed seed %d"), numSeeds);

	return 1;
}

#pragma endregion

int VoronoiGraph::CalcManhattanDistance(Point p1, struct Point p2)
{
	ModPoints(&p1);
	ModPoints(&p2);

	int distX = abs(p1.x - p2.x);
	int distY = abs(p1.y - p2.y);
	int distZ = abs(p1.z - p2.z);

	return distX + distY + distZ;
}

int VoronoiGraph::CalcPythagoreanDistance(Point p1, struct Point p2)
{
	ModPoints(&p1);
	ModPoints(&p2);

	int distX = (p1.x - p2.x);
	int distY = (p1.y - p2.y);
	int distZ = (p1.z - p2.z);

	return distX * distX + distY * distY + distZ * distZ;
}

//Determine location of seeds for level generation
//Places seeds centered around the given origin point
void VoronoiGraph::ScatterSeeds(int numSeedsInRegion, Point smallCorner)
{
	//UE_LOG(LogTemp, Warning, TEXT("SCATTERING SEEDS"));
	//Place the seeds on the voronoi graph
	for (int i = 0; i < numSeedsInRegion; i++)
	{
		int success = 0;

		while (!success)
		{
			struct Point point(
				rand() % loadedHorizontal + smallCorner.x,
				rand() % loadedHorizontal + smallCorner.y,
				rand() % loadedVertical + smallCorner.z);
			
			success = AddSeed(point);
		}
	}	
}

void VoronoiGraph::FindRegionsFromSeeds(Point smallCorner)
{
	//UE_LOG(LogTemp, Warning, TEXT("FINDING REGIONS FROM SEEDS"));
	for (int i = smallCorner.x; i < smallCorner.x + loadedHorizontal; i++)
	{
		for (int k = smallCorner.y; k < smallCorner.y + loadedHorizontal; k++)
		{
			for (int z = smallCorner.z; z < smallCorner.z + loadedVertical; z++)
			{
				int shortestDist = (2 * loadedHorizontal) + loadedVertical + 1;
				int shortestIndex = 0;
				Point p1(i, k, z);
				for (int j = 0; j < numSeeds; j++)
				{
					//int dist = CalcPythagoreanDistance(p1, voronoi.seedsList[j]);
					//UE_LOG(LogTemp, Warning, TEXT("Distance is %d"), dist);
					//UE_LOG(LogTemp, Warning, TEXT("POINT BEFORE IS (%d, %d, %d)"), p1.x, p1.y, p1.z);
					int dist = CalcManhattanDistance(p1, seedsList[j]);
					//UE_LOG(LogTemp, Warning, TEXT("POINT AFTER IS (%d, %d, %d)"), p1.x, p1.y, p1.z);
					if (dist < shortestDist)
					{
						shortestDist = dist;
						shortestIndex = j;
					}
				}
				SetLevelPoint(p1, shortestIndex);
			}
		}
	}
}

void VoronoiGraph::FillLevelGraph(int seedsPerRegion, Point smallCorner)
{
	ScatterSeeds(seedsPerRegion, smallCorner);

	FindRegionsFromSeeds(smallCorner);
}

//Uses levelGraph to fill edgesGraph
void VoronoiGraph::ConvolutionalTrace(Point smallCorner)
{
	//UE_LOG(LogTemp, Warning, TEXT("TRACING EDGES"));
	int neighbors[6] = { 0, 0, 0, 0, 0, 0 };
	int regions;
	//Iterate through the graph
	for (int i = smallCorner.x + 1; i < smallCorner.x + loadedHorizontal - 1; i++)
	{
		for (int k = smallCorner.y + 1; k < smallCorner.y + loadedHorizontal - 1; k++)
		{
			for (int j = smallCorner.z + 1; j < smallCorner.z + loadedVertical - 1; j++)
			{
				regions = 0;

				neighbors[0] = GetLevelPoint(i - 1, k, j);
				neighbors[1] = GetLevelPoint(i + 1, k, j);
				neighbors[2] = GetLevelPoint(i, k - 1, j);
				neighbors[3] = GetLevelPoint(i, k + 1, j);
				neighbors[4] = GetLevelPoint(i, k, j - 1);
				neighbors[5] = GetLevelPoint(i, k, j + 1);

				//Count how many unique regions are next to the point
				for (int x = 1; x < 6; x++)
				{
					int y;
					for (y = 0; y < x; y++)
						if (neighbors[x] == neighbors[y])
							break;
					
					if (x == y)
						regions++;
				}
				SetEdgesPoint(Point(i, k, j), regions);
			}
		}
	}
}

VoronoiGraph::~VoronoiGraph()
{
}


