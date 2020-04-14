#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"
#include "Animation.h"
#include "SpriteSheet.h"


class Bullet : public DisplayObject
{
public:
	Bullet(glm::vec2 position, float heading, int damage);
	~Bullet();

	// DisplayObject LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void spawn();
	bool m_checkBounds();
	int getDamage();
private:
	void m_changeDirection();
	void buildsprite();
	int m_currentFrame;
	int m_currentRow;
	SpriteSheet* m_pSpriteSheet;
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	std::unordered_map<std::string, Animation> m_pAnimations;
	int m_iDamage;
};

#endif /* defined (__CAT__) */