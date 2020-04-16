#pragma once
#ifndef __Melee_Enemy__
#define __Melee_Enemy__
#include "Enemy.h"
#include "Collider.h"

class Melee_Enemy final : public Enemy
{
private:
	

public:
	virtual void update() override;
	Melee_Enemy();
	virtual void draw() override;
	void m_buildAnimations();

	virtual void m_attack() override;
	virtual void m_idle() override;
	virtual void m_patrol() override;
	virtual void m_cower() override;
	virtual void m_checkHealth() override;

};

#endif // !__Melee_Enemy__