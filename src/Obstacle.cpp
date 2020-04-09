#include "Obstacle.h"
#include "Game.h"
#include "Util.h"

Obstacle::Obstacle()
{
	TheTextureManager::Instance()->load("../Assets/sprites/Obstacle.png",
		"obstacle", TheGame::Instance()->getRenderer());

	const auto size = TheTextureManager::Instance()->getTextureSize("obstacle");
	setWidth(64);
	setHeight(32);
	const float x = (rand() % 800 - getWidth());
	const float y = (rand() % 600 - getHeight());
	setPosition(glm::vec2(x,y));
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
