#include "Player.h"
#include "Game.h"
#include "Util.h"
#include <iostream>

Player::Player()
	: m_currentAnimationState(PLAYER_IDLE), m_iTotalHealth(100), m_iCurrentHealth(75), m_HealthBar(*this, m_iCurrentHealth, m_iTotalHealth),
	m_fScaleFactor(0.8)
{
	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", "spritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("spritesheet");
	m_buildAnimations();
	// set frame width
	setWidth(60);
	currentcounter = 0;
	canMelee = true;
	canShoot = true;
	// set frame height
	setHeight(60);
	m_meleeCollisionBox = new Collider;
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(PLAYER);
	m_meleeCollisionBox->setPosition(glm::vec2{500,1000});
	m_meleeCollisionBox->setHeight(65);
	m_meleeCollisionBox->setWidth(65);
	m_maxSpeed = 5.0f;
	m_currentHeading = 0.0f;
	m_currentDirection = glm::vec2(1.0f, 0.0f);
	m_turnRate = 20.0f;

	bulletspawnPos = glm::vec2(getPosition().x + 64, getPosition().y + 26);
	directionvector = { bulletspawnPos.x - getPosition().x, bulletspawnPos.y - getPosition().y };
	mag = Util::magnitude(directionvector);

	m_iRangedDamage = 10;
	m_iMeleeDamage = 25;

	m_iMeleeFrame = 0;
	m_iMeleeFramesMax = 104;
	m_iRangedFrame = 0;
	m_iRangedFramesMax = 39;

	m_playerControlState = true;
}

Player::~Player()
= default;

void Player::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["idle"].m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case PLAYER_RUN:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["run"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["run"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case PLAYER_MELEE:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["melee"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["melee"].m_currentFrame, 1.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case PLAYER_SHOOT:
		TheTextureManager::Instance()->playAnimation("spritesheet", m_pAnimations["shoot"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["shoot"].m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
	}

	for (int i = 0; i < m_pBulletvec.size(); i++)
	{
		if(m_pBulletvec[i] != nullptr)
		m_pBulletvec[i]->draw();
	}

	m_HealthBar.draw();

	if (TheGame::Instance()->getDebugMode())
	{
		glm::vec2 directionvector = bulletspawnPos - getPosition();
		float mag = Util::magnitude(directionvector);
		glm::vec2 dv2 = Util::normalize(directionvector);
		Util::DrawLine(getPosition(), getPosition() + dv2 * mag);
		Util::DrawLine(getPosition(), getPosition() + m_currentDirection * mag);
		Util::DrawCircle(getPosition(), getHeight() * 0.7);
		Util::DrawCircle(m_meleeCollisionBox->getPosition(), m_meleeCollisionBox->getWidth());
	}
}

void Player::update()
{
	if (m_playerControlState == true)
	{
		mouseLook();
	}
	accelerate();
	move();
	m_checkBounds();

	if (m_pAnimations["melee"].m_currentFrame == (int)m_pAnimations["melee"].frames.size() - 1)
	{
		m_pAnimations["melee"].m_currentFrame = 0;
		setAnimationState(PLAYER_IDLE);
	}

	if (m_pAnimations["shoot"].m_currentFrame == (int)m_pAnimations["shoot"].frames.size() - 1)
	{
		m_pAnimations["shoot"].m_currentFrame = 0;
		setAnimationState(PLAYER_IDLE);
	}


	if (m_iMeleeFrame == m_iMeleeFramesMax - 1)
	{
		m_meleeCollisionBox->setPosition(glm::vec2{ 500,1000 });
		canMelee = true;
	}
	else
	{
		m_iMeleeFrame++;
	}

	if (m_iRangedFrame == m_iRangedFramesMax - 1)
	{
		canShoot = true;
	}
	else
	{
		m_iRangedFrame++;
	}

	for (int i = 0; i < (int)m_pBulletvec.size(); i++)
	{
		m_pBulletvec[i]->update();
		if (m_pBulletvec[i]->m_checkBounds())
		{
			delete m_pBulletvec[i];
			m_pBulletvec[i] = nullptr;
			m_pBulletvec.erase(m_pBulletvec.begin() + i);
		}
	}

	updatebulletspawn();
	m_HealthBar.update();
	//std::cout << bulletspawnPos.x <<" " << bulletspawnPos.y  << std::endl;
}

void Player::clean()
{
}

void Player::move()
{
	setPosition(getPosition() + getVelocity());
	setVelocity(getVelocity() * 0.95f);

	bulletspawnPos += getVelocity();
}

void Player::m_checkBounds()
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

std::vector<Bullet*>& Player::getBullets()
{
	return m_pBulletvec;
}

void Player::moveForward()
{
	setAcceleration(m_currentDirection * m_maxSpeed);
	//setVelocity(m_currentDirection * m_maxSpeed);
}

void Player::moveBack()
{
	setAcceleration(m_currentDirection * -m_maxSpeed);
	//setVelocity(m_currentDirection * -m_maxSpeed);
}

void Player::turnRight()
{
	m_currentHeading += m_turnRate;
	if (m_currentHeading >= 360)
	{
		m_currentHeading -= 360.0f;
	}

	m_changeDirection();
}

void Player::turnLeft()
{
	m_currentHeading -= m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}

	m_changeDirection();

}

void Player::turnaround()
{
	m_currentHeading -= 180.0f;
		if (m_currentHeading < 0)
		{
			m_currentHeading += 360.0f;
		}
	m_changeDirection();
}

void Player::mouseLook()
{
	glm::vec2 playrPos = this->getPosition();
	glm::vec2 mousePos = TheGame::Instance()->getMousePosition();
	glm::vec2 relative = Util::normalize(mousePos - playrPos);
	m_currentDirection = relative;

	float angle = -(SDL_atan2f(relative.x, relative.y) * Util::Rad2Deg) + 90;
	m_currentHeading = angle;
}

void Player::updatebulletspawn()
{
	glm::vec2 tempos2 = Util::rotateVector(m_currentDirection,24.785);
	tempos2 = Util::normalize(tempos2);
	bulletspawnPos = { getPosition().x + (tempos2.x * mag), getPosition().y + (tempos2.y * mag) };

}

void Player::melee()
{
		canMelee = false;
		float angle = 0.0f;
		setAnimationState(PLAYER_MELEE);
		glm::vec2 tempos = Util::rotateVector(m_currentDirection, angle);
		tempos = Util::normalize(tempos);
		m_meleeCollisionBox->setPosition(glm::vec2{ getPosition().x + (tempos.x * 50.0f), getPosition().y + (tempos.y * 50.0f) });
		m_iMeleeFrame = 0;
	
}

void Player::shoot()
{
	canShoot = false;
	m_pBulletvec.push_back(new Bullet(bulletspawnPos, getHeading(), m_iRangedDamage));
	//m_pBulletvec.back()->spawn();
	m_iRangedFrame = 0;
}

bool Player::changeHealth(int change)
{
	m_iCurrentHealth += change;
	if (m_iCurrentHealth <= 0)
	{
		return true;
	}
	return false;
}

bool Player::getControlState() const
{
	return m_playerControlState;
}

void Player::setControlState(bool state)
{
	m_playerControlState = state;
}

void Player::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ship");
}

void Player::accelerate()
{
	glm::vec2 newVelocity = getVelocity() + getAcceleration();
	if (Util::magnitude(newVelocity) > m_maxSpeed)
	{
		newVelocity = Util::normalize(newVelocity) * m_maxSpeed;
	}

	setVelocity(newVelocity);
	setAcceleration({ 0, 0 });
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
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-idle-0"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-idle-1"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-idle-2"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-idle-3"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-idle-4"));

	m_pAnimations["idle"] = idleAnimation;

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-0"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-1"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-2"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-3"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-4"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-5"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-run-6"));
	m_pAnimations["run"] = runAnimation;

	Animation meleeAnimation = Animation();

	meleeAnimation.name = "melee";
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-0"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-1"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-2"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-3"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-4"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-5"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-6"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-7"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-8"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-9"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-10"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-11"));
	meleeAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-melee-12"));
	m_pAnimations["melee"] = meleeAnimation;

	Animation shootAnimation = Animation();

	shootAnimation.name = "shoot";
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-shoot-0"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-shoot-1"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("survivor-shoot-2"));
	m_pAnimations["shoot"] = shootAnimation;
}
