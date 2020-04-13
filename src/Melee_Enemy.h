#pragma once
#ifndef __Melee_Enemy__
#define __Melee_Enemy__
#include "Enemy.h"

class Melee_Enemy final : public Enemy
{
public:
	Melee_Enemy();
	void draw();
};

#endif // !__Ranged_Enemy__