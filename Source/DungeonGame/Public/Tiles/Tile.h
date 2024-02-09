// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Connections.h"

/**
 * 
 */
class DUNGEONGAME_API Tile
{

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	Connections* selfConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	Tile* ValidNeighbors;


public:
	Tile();
	~Tile();

	virtual void GetMesh() = 0;

	virtual Connections* GetConnections() = 0;

	virtual Tile* GetValidNeighbors() = 0;

};
