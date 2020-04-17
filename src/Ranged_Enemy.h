#pragma once
#ifndef __Ranged_Enemy__
#define __Ranged_Enemy__
#include "Enemy.h"
#include "Bullet.h"

class Ranged_Enemy final : public Enemy
{
public:
	struct Loadout
	{
		SDL_Colour colour;
		float scale;
		int health;
		int damage;
		float smell;
		float FOV;
		int value;
		Loadout(SDL_Colour colour = { 255, 255, 255, 255 }, float scale = 2.0f, int health = 80, int damage = -10, float smell = 100.0f, float FOV = 40.0f, int value = 20)
			: colour(colour), scale(scale), health(health), damage(damage), smell(smell), FOV(FOV), value(value) {}
	};
private:
	bool quarterHealth[3];
	float playerDistance;
	std::vector<Bullet*> m_pBulletvec;
	bool canShoot;
	int m_shootFrame;
	int m_shootFrameMax;
	glm::vec2 bulletspawnPos;
	glm::vec2 directionvector;
	float mag;
	void updatebulletspawn();
public:
	virtual void update() override;
	Ranged_Enemy(Ranged_Enemy::Loadout& loadout);
	virtual void draw() override;
	void m_buildAnimations();

	virtual void m_attack() override;
	virtual void m_idle() override;
	virtual void m_patrol() override;
	virtual void m_cower() override;
	virtual void m_checkHealth() override;
};

#endif // !__Ranged_Enemy__