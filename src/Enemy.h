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
	//void accelerate();
	//void decelerate();
	void m_checkBounds();
	glm::vec2 getFeelerEndPosition(unsigned int feeler_number);
	void setFeeler(unsigned int feeler_number, bool value);
	void setTargetPosition(glm::vec2 target_position);
	
	// Detection variables
	void setLOS(bool value);
	bool hasLOS() const;
	float getSmellRadius();
	void setSmell(bool value);
	bool hasSmell() const;
	bool canDetect() const;

private:
	float m_fScaleFactor;
	void m_buildAnimations();

	SpriteSheet* m_pSpriteSheet;
	WolfAnimationState m_currentAnimationState;
	BehaviourState m_Behaviour;
	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_maxSpeed;
	float m_turnRate;
	void m_turn(float angle);
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	glm::vec2 getTargetPosition();

	// Health and bar
	int m_iTotalHealth;
	int m_iCurrentHealth;
	HealthBar m_HealthBar;

	// DEcision making Functions
	BehaviourState getBehaviour();
	void setBehaviour(BehaviourState state);
	void m_checkBehaviourState();

	// steering behaviour functions
	void m_checkSteeringState();
	void m_seek();
	void m_flee();
	void m_checkArrival();
	bool m_checkFeelers();
	void m_arrive();

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
	int m_numFramesAvoiding;
	int m_maxFramesAvoiding;

	// target info
	float m_angleToTarget;
	float m_distanceToTarget;
	float m_arrivalRadius;
	float m_arrivalTarget;
	glm::vec2 m_targetDirection;

	bool m_hasLOS;
	bool m_hasSmell;
	float m_smellRadius;
};


#endif /* defined (__ENEMY__) */