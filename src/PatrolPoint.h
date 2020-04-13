#pragma once
#ifndef __PATROLPOINT__
#define __PATROLPOINT__
#include "PathFindingDisplayObject.h"
#include "Animation.h"
#include <unordered_map>
#include "SpriteSheet.h"

class PatrolPoint : public PathFindingDisplayObject
{
public:
	PatrolPoint();
	~PatrolPoint();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void reset();

	// getters and setters
	bool isActive();
	void setActive();
	
private:
	void m_buildAnimations();
	
	SpriteSheet* m_pSpriteSheet;

	int m_currentFrame;
	bool m_bIsActive;

	std::unordered_map<std::string, Animation> m_pAnimations;
};

#endif /* defined (__EXPLOSION__) */