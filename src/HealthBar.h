#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include <SDL.h>
#include "GameObject.h"

class HealthBar
{
private:
	int& m_WatchTotal;
	int& m_WatchCurrent;
	GameObject& m_WatchObject;
	SDL_Rect m_rFrame;
	SDL_Rect m_rFill;
	SDL_Colour m_cFrameColour;
	SDL_Colour m_cFillColour;
public:
	HealthBar(GameObject& object, int& current, int& total, float barscale = 1.0f, SDL_Colour fillcolour = { 128, 255, 96, 192 }, SDL_Colour framecolour = { 128, 128, 128, 128 });
	void update();
	void draw();
};

#endif // !__HEALTH_BAR__