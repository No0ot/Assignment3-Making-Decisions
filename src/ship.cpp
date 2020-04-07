#include "ship.h"
#include "Game.h"
#include "Util.h"
#include "PlayScene.h"
#include "glm/gtx/string_cast.hpp"


Ship::Ship() : m_maxSpeed(10.0f)
{
	TheTextureManager::Instance()->load("../Assets/textures/ship3.png",
		"ship", TheGame::Instance()->getRenderer());

	auto size = TheTextureManager::Instance()->getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(SHIP);
	setState(IDLE);
	
	m_currentHeading = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame
}


Ship::~Ship()
= default;

void Ship::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("ship", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
}


void Ship::update()
{
	move();
	m_checkBounds();
}

void Ship::clean()
{
}

void Ship::turnRight()
{
	m_currentHeading += m_turnRate;
	if (m_currentHeading >= 360) 
	{
		m_currentHeading -= 360.0f;
	}
	m_changeDirection();
}

void Ship::turnLeft()
{
	m_currentHeading -= m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}

	m_changeDirection();
}

void Ship::moveForward()
{
	setVelocity(m_currentDirection * m_maxSpeed);
}

void Ship::moveBack()
{
	setVelocity(m_currentDirection * -m_maxSpeed);
}

void Ship::move()
{
	setPosition(getPosition() + getVelocity());
	setVelocity(getVelocity() * 0.9f);

}

glm::vec2 Ship::getTargetPosition()
{
	return m_targetPosition;
}

glm::vec2 Ship::getCurrentDirection()
{
	return m_currentDirection;
}

float Ship::getMaxSpeed()
{
	return m_maxSpeed;
}

void Ship::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;

}

void Ship::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}



void Ship::m_checkBounds()
{

	if (getPosition().x > Config::SCREEN_WIDTH)
	{
		setPosition(glm::vec2(0.0f, getPosition().y));
	}

	if (getPosition().x < 0)
	{
		setPosition(glm::vec2(800.0f, getPosition().y));
	}

	if (getPosition().y > Config::SCREEN_HEIGHT)
	{
		setPosition(glm::vec2(getPosition().x, 0.0f));
	}

	if (getPosition().y < 0)
	{
		setPosition(glm::vec2(getPosition().x, 600.0f));
	}

}

void Ship::m_reset()
{
	setIsColliding(false);
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	setPosition(glm::vec2(xComponent, yComponent));
}

void Ship::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ship");
}

