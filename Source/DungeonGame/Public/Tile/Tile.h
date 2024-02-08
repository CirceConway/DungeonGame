// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DUNGEONGAME_API Tile
{
public:
	Tile();
	~Tile();

	virtual void Spawn() = 0;
};
