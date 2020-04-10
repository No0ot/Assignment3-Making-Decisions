#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "PathFindingDisplayObject.h"
#include "Animation.h"
#include "SpriteSheet.h"
#include "AnimationState.h"
#include "Util.h"
#include <unordered_map>

class Enemy final : public PathFindingDisplayObject
{
public:
	Enemy();
	~Enemy();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void move();
	void m_checkBounds();
private:
	void m_buildAnimations();

	SpriteSheet* m_pSpriteSheet;
	WolfAnimationState m_currentAnimationState;
	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_maxSpeed;
	float m_turnRate;
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;


};


#endif /* defined (__ENEMY__) */