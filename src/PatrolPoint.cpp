#include "PatrolPoint.h"
#include "Game.h"
#include <utility>

PatrolPoint::PatrolPoint() : m_currentFrame(0), m_bIsActive(false)
{

	// set frame width
	setWidth(52);

	// set frame height
	setHeight(52);

	setIsColliding(false);
	setType(PATROL_POINT);

}

PatrolPoint::~PatrolPoint()
= default;

void PatrolPoint::draw()
{
	
}

void PatrolPoint::update()
{
}

void PatrolPoint::clean()
{
}

void PatrolPoint::reset()
{

}

bool PatrolPoint::isActive()
{
	return m_bIsActive;
}

void PatrolPoint::setActive()
{
	m_currentFrame = 0;
	m_bIsActive = true;
}

