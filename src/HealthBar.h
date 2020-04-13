#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include <SDL.h>
#include "GameObject.h"

class Bar
{
protected:
	int& m_WatchTotal;
	int& m_WatchCurrent;
	SDL_Rect m_rFrame;
	SDL_Rect m_rFill;
	SDL_Colour m_cFrameColour;
	SDL_Colour m_cFillColour;
public:
	Bar(int& current, int& total, float barscale = 1.0f, SDL_Colour fillcolour = { 128, 255, 96, 192 }, SDL_Colour framecolour = { 128, 128, 128, 128 });
	virtual void update();
	virtual void draw();
};

class HealthBar : public Bar
{
private:
	GameObject& m_WatchObject;
public:
	HealthBar(GameObject& object, int& current, int& total, float barscale = 1.0f, SDL_Colour fillcolour = { 128, 255, 96, 192 }, SDL_Colour framecolour = { 128, 128, 128, 128 });
	void update() override;
};

class UIBar : public Bar
{
private:
	int m_iXPos, m_iYPos;
public:
	UIBar(int x, int y, int& current, int& total, float barscale = 1.0f, SDL_Colour fillcolour = { 128, 255, 96, 192 }, SDL_Colour framecolour = { 128, 128, 128, 128 });
};

#endif // !__HEALTH_BAR__