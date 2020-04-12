#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "PathFindingDisplayObject.h"
#include "Animation.h"
#include "SpriteSheet.h"
#include "AnimationState.h"
#include "Util.h"
#include <unordered_map>
#include "HealthBar.h"

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
	void accelerate();
	void decelerate();
	void m_checkBounds();
	glm::vec2 getFeelerEndPosition(unsigned int feeler_number);
	void setFeeler(unsigned int feeler_number, bool value);
	void setTargetPosition(glm::vec2 target_position);
private:
	float m_fScaleFactor;
	void m_buildAnimations();

	SpriteSheet* m_pSpriteSheet;
	WolfAnimationState m_currentAnimationState;
	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_maxSpeed;
	float m_turnRate;
	void m_turn(float direction, bool to_target = true);
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	glm::vec2 getTargetPosition();

	// Health and bar
	int m_iTotalHealth;
	int m_iCurrentHealth;
	HealthBar m_HealthBar;

	// steering behaviour functions
	void m_checkSteeringState();
	void m_seek();
	void m_flee();
	void m_checkArrival();
	void m_checkCollisions();
	void m_arrive();
	void m_avoid();

	// ship movement
	int m_turnFrame;
	int m_turnFrameMax;
	void m_reorient();

	// ship avoidance info
	float m_feelerAngle;
	bool m_feelerCol[3];
	float m_feelerLength;
	glm::vec2 m_leftFeelerPos;
	glm::vec2 m_rightFeelerPos;
	float m_avoidDirection;
	int m_avoidEndFrame;
	int m_avoidEndFrameMax;
	bool m_checkFeelers();

	// target info
	float m_angleToTarget;
	float m_distanceToTarget;
	float m_arrivalRadius;
	float m_arrivalTarget;
	glm::vec2 m_targetDirection;
};


#endif /* defined (__ENEMY__) */