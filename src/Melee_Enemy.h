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
	void draw();
	void m_buildAnimations();

	virtual void m_attack() override;

};

#endif // !__Melee_Enemy__