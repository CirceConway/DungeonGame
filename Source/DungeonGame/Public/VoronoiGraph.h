// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoronoiGraph.generated.h"

/**
 * 
 */

USTRUCT()
struct Point
{

	GENERATED_BODY()

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

	int Val(int val)
	{
		switch (val)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return -1;
		}
	}
};

UCLASS()
class DUNGEONGAME_API VoronoiGraph
{

	GENERATED_BODY()

public:
	/*
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

		//For accessing the point values in an array-like manner
		int Val(int val)
		{
			switch (val) 
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return -1;
			}
		}

	};*/

private:
	
	int maxSeeds;
	int loadedHorizontal, loadedVertical;

	int horizontalSquare;

	int* levelGraph;
	int* edgesGraph;

	//This is the only thing in the class that can just keep growing
	Point* seedsList;
	int numSeeds;

	void SetLevelPoint(Point p, int val);
	void SetEdgesPoint(Point p, int val);

	int AddSeed(Point p);

	void ModPoints(Point* p) const;

public:
	VoronoiGraph(int loadedHorizontal, int loadedVertical, int maxSeeds);

	void ScatterSeeds(int numSeedsInRegion, Point origin);

	void FindRegionsFromSeeds(Point smallCorner);

	void FillLevelGraph(int seedsPerRegion, Point smallCorner);

	void ConvolutionalTrace(Point smallCorner);
	
	
	
	int CalcManhattanDistance(Point p1, Point p2);

	int CalcPythagoreanDistance(Point p1, Point p2);

	

	int GetLevelPoint(Point p);
	int GetLevelPoint(int x, int y, int z);

	int GetEdgesPoint(Point p);
	
	~VoronoiGraph();
};
