#include "Obstacle.h"
#include "Game.h"
#include "Util.h"

Obstacle::Obstacle()
{
	TheTextureManager::Instance()->load("../Assets/sprites/ObstacleL.png",
		"obstacle", TheGame::Instance()->getRenderer());

	const auto size = TheTextureManager::Instance()->getTextureSize("obstacle");
	setWidth(80);
	setHeight(80);
	setPosition(glm::vec2(0.0f,0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setType(OBSTACLE);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("obstacle", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0, 255, true);

	glm::vec2 temp = { getPosition().x - getWidth() * 0.5, getPosition().y - getHeight() * 0.5 };
	Util::DrawRect(temp, getWidth(), getHeight());
}

void Obstacle::update()
{

}

void Obstacle::clean()
{
}
