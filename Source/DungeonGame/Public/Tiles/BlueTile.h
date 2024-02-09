// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/Tile.h"

/**
 * 
 */
class DUNGEONGAME_API BlueTile : Tile
{
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	Connections* selfConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	Tile* ValidNeighbors;

public:
	BlueTile();
	~BlueTile();

	virtual void GetMesh() override;

	virtual Connections* GetConnections() override;

	virtual Tile* GetValidNeighbors() override;

};
