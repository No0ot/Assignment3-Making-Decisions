#pragma once
#ifndef __Melee_Enemy__
#define __Melee_Enemy__
#include "Enemy.h"

class Melee_Enemy final : public Enemy
{
private:
	
public:
	Melee_Enemy();
	void draw();
	void m_buildAnimations();

};

#endif // !__Melee_Enemy__