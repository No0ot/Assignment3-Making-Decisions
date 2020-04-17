#pragma once
#ifndef __Melee_Enemy__
#define __Melee_Enemy__
#include "Enemy.h"
#include "Collider.h"

class Melee_Enemy final : public Enemy
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
		Loadout(SDL_Colour colour = { 255, 255, 255, 255 }, float scale = 1.3f, int health = 50, int damage = -10, float smell = 100.0f, float FOV = 40.0f, int value = 10)
			: colour(colour), scale(scale), health(health), damage(damage), smell(smell), FOV(FOV), value(value) {}
	};
private:
	

public:
	virtual void update() override;
	Melee_Enemy(Melee_Enemy::Loadout& loadout);
	virtual void draw() override;
	void m_buildAnimations();

	virtual void m_attack() override;
	virtual void m_idle() override;
	virtual void m_patrol() override;
	virtual void m_cower() override;
	virtual void m_checkHealth() override;

};

#endif // !__Melee_Enemy__