#include "Explosion.h"
#include "Game.h"
#include <utility>

Explosion::Explosion() : m_currentFrame(0), m_bIsActive(false)
{
	TheTextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png",
		"spritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("spritesheet");

	// set frame width
	setWidth(52);

	// set frame height
	setHeight(52);

	setIsColliding(false);
	setType(EXPLOSION);

	m_buildAnimations();

	reset();
}

Explosion::~Explosion()
= default;

void Explosion::draw()
{
	if(m_currentFrame < m_pAnimations["explosion"].frames.size() - 1)
	{
		TheTextureManager::Instance()->playAnimation(
			"spritesheet", m_pAnimations["explosion"],
			getPosition().x, getPosition().y, m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), 0, 255, true);
	}
	else
	{
		reset();
	}
	
}

void Explosion::update()
{
}

void Explosion::clean()
{
}

void Explosion::reset()
{
	setPosition(-1000.0f, -1000.0f);
	m_bIsActive = false;
}

bool Explosion::isActive()
{
	return m_bIsActive;
}

void Explosion::setActive()
{
	m_currentFrame = 0;
	m_bIsActive = true;
}

void Explosion::m_buildAnimations()
{
	Animation explosionAnimation = Animation();

	explosionAnimation.name = "explosion";
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion1"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion2"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion3"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion4"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion5"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion6"));
	explosionAnimation.frames.push_back(m_pSpriteSheet->getFrame("explosion7"));

	m_pAnimations["explosion"] = explosionAnimation;
}
