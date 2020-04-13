#pragma once
#ifndef __Ranged_Enemy__
#define __Ranged_Enemy__
#include "Enemy.h"

class Ranged_Enemy final : public Enemy
{
public:
	Ranged_Enemy();
	void draw();
};

#endif // !__Ranged_Enemy__