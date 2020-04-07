#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "DisplayObject.h"
#include "PlayerAnimationState.h"
#include "Animation.h"
#include "SpriteSheet.h"
#include <unordered_map>

class Player : public DisplayObject
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(PlayerAnimationState new_state);
	void setAnimation(const Animation& animation);

	//Animation getAnimation() { return m_pAnimations; }

	void moveForward();
	void moveBack();
	void turnRight();
	void turnLeft();

	void move();
	void m_checkBounds();
	
private:
	void m_buildAnimations();

	SpriteSheet* m_pSpriteSheet;

	

	PlayerAnimationState m_currentAnimationState;
	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_maxSpeed;
	float m_turnRate;
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};

#endif /* defined (__PLAYER__) */