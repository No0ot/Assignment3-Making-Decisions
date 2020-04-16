#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PathFindingDisplayObject.h"
#include "Collider.h"
#include "AnimationState.h"
#include "Animation.h"
#include "SpriteSheet.h"
#include "Bullet.h"
#include <unordered_map>
#include "HealthBar.h"

class Player : public PathFindingDisplayObject
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

	//getters
	float getHeading() { return m_currentHeading; }

	void moveForward();
	void moveBack();
	void turnRight();
	void turnLeft();
	void turnaround();
	void mouseLook();

	void updatebulletspawn();
	void melee();
	void shoot();

	void move();
	void m_checkBounds();
	std::vector<Bullet*>& getBullets();
	Collider* getCollider() { return m_meleeCollisionBox; }
	int m_iMeleeDamage;
	PlayerAnimationState getAnimationState() { return m_currentAnimationState;  }
	bool changeHealth(int cdamage);
	bool canMelee;
	bool canShoot;
	bool getControlState() const;
	void setControlState(bool state);
private:
	int currentcounter;
	int currentcounterMax;
	float m_fScaleFactor;
	void m_buildAnimations();
	Collider* m_meleeCollisionBox;

	SpriteSheet* m_pSpriteSheet;
	PlayerAnimationState m_currentAnimationState;
	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_maxSpeed;
	float m_turnRate;
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	glm::vec2 bulletspawnPos;
	glm::vec2 directionvector;
	float mag;
	void accelerate();

	int m_iTotalHealth;
	int m_iCurrentHealth;
	HealthBar m_HealthBar;

	unsigned int m_iMeleeFrame;
	unsigned int m_iMeleeFramesMax;
	unsigned int m_iRangedFrame;
	unsigned int m_iRangedFramesMax;
	int m_iRangedDamage;
	std::vector<Bullet*> m_pBulletvec;

	bool m_playerControlState;
};

#endif /* defined (__PLAYER__) */