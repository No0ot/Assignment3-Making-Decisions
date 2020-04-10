#pragma once
#ifndef __ANIMATION_STATE__
#define __ANIMATION_STATE__

enum PlayerAnimationState
{
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_SHOOT,
	PLAYER_MELEE,
	NUM_OF_ANIMATION_STATES
};

enum WolfAnimationState
{
	WOLF_IDLE,
	WOLF_WALK,
	WOLF_RUN,
	WOLF_BITE,
	NUM_OF_WOLFANIMATION_STATES
};

#endif /* defined (__ANIMATION_STATE__)*/