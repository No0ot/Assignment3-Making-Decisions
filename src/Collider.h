#pragma once
#ifndef __COLLIDER__
#define __COLLIDER__

#include "GameObject.h"
#include "Animation.h"
#include "SpriteSheet.h"


class Collider : public GameObject
{
public:
	Collider();
	~Collider();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
private:
	
};

#endif /* defined (__CAT__) */