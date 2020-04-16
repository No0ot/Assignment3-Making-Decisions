#pragma once
#ifndef __Ranged_Enemy__
#define __Ranged_Enemy__
#include "Enemy.h"

class Ranged_Enemy final : public Enemy
{
private:
	bool quarterHealth[3];
	float playerDistance;
public:
	virtual void update() override;
	Ranged_Enemy();
	virtual void draw() override;
	void m_buildAnimations();

	virtual void m_attack() override;
	virtual void m_idle() override;
	virtual void m_patrol() override;
	virtual void m_cower() override;
	virtual void m_checkHealth() override;
};

#endif // !__Ranged_Enemy__