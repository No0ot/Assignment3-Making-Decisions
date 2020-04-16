#pragma once
#ifndef __Ranged_Enemy__
#define __Ranged_Enemy__
#include "Enemy.h"

class Ranged_Enemy final : public Enemy
{
private:
public:
	virtual void update() override;
	Ranged_Enemy();
	virtual void draw() override;
	void m_buildAnimations();

	virtual void m_attack() override;
};

#endif // !__Ranged_Enemy__