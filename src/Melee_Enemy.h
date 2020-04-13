#pragma once
#ifndef __Melee_Enemy__
#define __Melee_Enemy__
#include "Enemy.h"

class Melee_Enemy final : public Enemy
{
private:
	WolfAnimationState m_currentAnimationState;
public:
	Melee_Enemy();
	void draw();
	void m_buildAnimations();
	WolfAnimationState getAnimState();
	void setAnimState(WolfAnimationState state);
};

#endif // !__Melee_Enemy__