#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "PathFindingDisplayObject.h"
#include "Animation.h"
#include "Collider.h"
#include "SpriteSheet.h"
#include "AnimationState.h"
#include "BehaviourState.h"
#include "Util.h"
#include <unordered_map>
#include "HealthBar.h"

class Enemy : public PathFindingDisplayObject
{
public:
	Enemy();
	~Enemy();

	// Inherited via GameObject
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() override;

	//virtual enemy functions
	void m_checkBehaviourState();
	virtual void m_idle() = 0;
	virtual void m_patrol() = 0;
	virtual void m_attack() = 0;
	virtual void m_cower() = 0;
	virtual void m_checkHealth() = 0;

	void move();
	//void accelerate();
	//void decelerate();
	void m_checkBounds();
	glm::vec2 getFeelerEndPosition(unsigned int feeler_number);
	void setFeeler(unsigned int feeler_number, bool value);
	void setTargetPosition(glm::vec2 target_position);
	BehaviourState getBehaviour();
	void setBehaviour(BehaviourState state);
	void setNextPatrolPoint(glm::vec2 target_position);
	WolfAnimationState getAnimState();
	void setAnimState(WolfAnimationState state);
	int getMySpecialNumber() { return m_specialnumber; }

	// Detection variables
	void setLOS(bool value);
	bool hasLOS() const;
	float getSmellRadius();
	void setSmell(bool value);
	bool hasSmell() const;
	bool canDetect() const;
	float getFOV() const;
	glm::vec2 getDirection() const;
	
	// Damage variables
	int getDamage();
	bool changeHealth(int change);
	int getPtsValue();
	Collider* getCollider() { return m_meleeCollisionBox; }

	int randomnum;
	unsigned int DisplayListIndexInScene;
	bool canDamage;
	glm::vec2 m_playersPos;
	glm::vec2 m_nearestCoverTile;
	glm::vec2 m_furthestCoverTile;
	glm::vec2 m_patrolPoint;
protected:
	Collider* m_meleeCollisionBox;
	bool canCower;
	bool m_arrived;
	float m_fScaleFactor;
	

	SpriteSheet* m_pSpriteSheet;
	WolfAnimationState m_currentAnimationState;
	BehaviourState m_Behaviour;
	std::unordered_map<std::string, Animation> m_pAnimations;
	SDL_Colour m_tint;

	float m_maxSpeed;
	float m_turnRate;
	void m_turn(float angle);
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	glm::vec2 m_nextPatrolPoint;
	glm::vec2 getTargetPosition();

	// Health and bar
	int m_iTotalHealth;
	int m_iCurrentHealth;
	HealthBar* m_HealthBar;

	// DEcision making Functions



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

	int m_Stateframes;
	int m_StateframesMax;

	// target info
	int m_specialnumber;
	float m_angleToTarget;
	float m_distanceToTarget;
	float m_arrivalRadius;
	float m_arrivalTarget;
	glm::vec2 m_targetDirection;

	// detection info
	bool m_hasLOS;
	bool m_hasSmell;
	float m_smellRadius;
	float m_fFOV;

	// damage info
	int m_iDamage;
	int m_iPtsValue;
};

#endif /* defined (__ENEMY__) */