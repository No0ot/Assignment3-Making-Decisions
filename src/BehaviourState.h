#pragma once
#ifndef __BEHAVIOUR_STATE__
#define __BEHAVIOUR_STATE__
enum class BehaviourState {
	NO_STATE = -1,
	IDLE2,		// keep enemy at current position
	PATROL,		// seek to closest patrol  tile on list and than patrol between that tile and the next closest patrol tile
	ATTACK,		// Initiate attack command
	ASSAULT,	// Seek to player
	FLEE,		// Flee from player
	COWER,		// find the best cover tile and idle

};
#endif /* defined (__BEHAVIOUR_STATE__) */