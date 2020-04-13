#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	SHIP,
	TARGET,
	ENEMY,
	MELEE_ENEMY,
	RANGED_ENEMY,
	MINE,
	TILE,
	BULLET,
	PLAYER,
	PLANE,
	EXPLOSION,
	OBSTACLE,
	NUM_OF_TYPES
};
#endif /* defined (__GAME_OBJECT_TYPE__) */