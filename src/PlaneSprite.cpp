#include "PlaneSprite.h"
#include "Game.h"
#include <utility>

PlaneSprite::PlaneSprite() : m_currentFrame(0)
{
	TheTextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("spritesheet");

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	setPosition(glm::vec2(400.0f, 200.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(PLANE);

	m_buildAnimations();
}

PlaneSprite::~PlaneSprite()
= default;

void PlaneSprite::draw()
{
	TheTextureManager::Instance()->playAnimation(
		"spritesheet", m_pAnimations["plane"],
		getPosition().x, getPosition().y, m_currentFrame, 0.5f, 
		TheGame::Instance()->getRenderer(), 0, 255, true);
}

void PlaneSprite::update()
{
}

void PlaneSprite::clean()
{
}

void PlaneSprite::setAnimation(const Animation& animation)
{
	m_pAnimations[animation.name] = animation;
}

void PlaneSprite::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "plane";
	planeAnimation.frames.push_back(m_pSpriteSheet->getFrame("plane1"));
	planeAnimation.frames.push_back(m_pSpriteSheet->getFrame("plane2"));
	planeAnimation.frames.push_back(m_pSpriteSheet->getFrame("plane3"));

	m_pAnimations["plane"] = planeAnimation;
}





