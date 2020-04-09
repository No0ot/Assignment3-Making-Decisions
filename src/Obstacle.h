#pragma once
#ifndef __Obstacle__
#define __Obstacle__


#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"
#include "PathFindingDisplayObject.h"

class Obstacle final : public PathFindingDisplayObject
{
public:
     Obstacle();
	~Obstacle();

	// Inherited via GameObject
	virtual void draw() override;

	virtual void update() override;

	virtual void clean() override;

private:

};


#endif /* defined (__Mine__) */