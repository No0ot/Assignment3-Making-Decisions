#include "Player.h"
#include "Game.h"

Player::Player(): m_currentFrame(0), m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", "spritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("spritesheet");
	
	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(PLAYER);

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["run"],
			getPosition().x, getPosition().y, m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["run"],
			getPosition().x, getPosition().y, m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	}
	
	
	
}

void Player::update()
{
}

void Player::clean()
{
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::setAnimation(const Animation& animation)
{
	m_pAnimations[animation.name] = animation;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-idle-0"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-idle-1"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-idle-2"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-idle-3"));

	m_pAnimations["idle"] = idleAnimation;

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-run-0"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-run-1"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-run-2"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("megaman-run-3"));

	m_pAnimations["run"] = runAnimation;
}
