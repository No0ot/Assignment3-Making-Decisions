#include "Bullet.h"
#include "Game.h"
#include "Util.h"

Bullet::Bullet(glm::vec2 position, float heading, int damage) : m_iDamage(damage)
{
	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", "spritesheet", TheGame::Instance()->getRenderer());

	// framesize is 512 x 256
	
	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("spritesheet");
	setWidth(16);
	setHeight(16);

	setPosition(position);
	m_currentHeading = heading;
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(BULLET);
	m_currentFrame = 0;
	m_currentRow = 0;
	m_changeDirection();

	setVelocity(m_currentDirection * 10.0f);
	buildsprite();
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["shoot"],
		getPosition().x, getPosition().y, 1.0f, m_pAnimations["shoot"].m_currentFrame, 0.12f,
		TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
	
	if (TheGame::Instance()->getDebugMode())
	{
		glm::vec2 temp = { getPosition().x - getWidth() * 0.5, getPosition().y - getHeight() * 0.5 };
		Util::DrawRect(temp, getWidth(), getHeight());
	}
}

void Bullet::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}

void Bullet::update()
{
	
	setPosition(getPosition() + getVelocity());
}

void Bullet::clean()
{
}

bool Bullet::m_checkBounds()
{

	if (getPosition().x > Config::SCREEN_WIDTH || getPosition().x < 0 ||
		getPosition().y > Config::SCREEN_HEIGHT || getPosition().y < 0)
	{
		return true;
	}
	else
		return false;

}

int Bullet::getDamage()
{
	return m_iDamage;
}

void Bullet::buildsprite()
{
	Animation shootAnimation = Animation();

	shootAnimation.name = "shoot";
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Bullet"));

	m_pAnimations["shoot"] = shootAnimation;
}