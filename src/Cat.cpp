#include "Cat.h"
#include "Game.h"

Cat::Cat()
{
	TheTextureManager::Instance()->load("../Assets/textures/cat.png",
		"cat", TheGame::Instance()->getRenderer());

	// framesize is 512 x 256
	
	auto size = TheTextureManager::Instance()->getTextureSize("cat");
	setWidth(512);
	setHeight(256);

	std::cout << "width: " << size.x << " height: " << size.y << std::endl;
	setPosition(glm::vec2(400.0f, 300.0f));
	setIsColliding(false);
	setType(CAT);
	setState(IDLE);
	m_currentFrame = 0;
	m_currentRow = 0;
}

Cat::~Cat()
= default;

void Cat::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	TheTextureManager::Instance()->drawFrame("cat", xComponent, 
		yComponent, 512, 256, m_currentRow, 
		m_currentFrame, 4, 2, 1.0f,
		TheGame::Instance()->getRenderer(), 0, 255, true);	
}

void Cat::update()
{
}

void Cat::clean()
{
}
