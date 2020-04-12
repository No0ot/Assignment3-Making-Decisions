#include "Enemy.h"
#include "Game.h"

Enemy::Enemy() : m_currentAnimationState(WOLF_IDLE), m_iTotalHealth(50), m_iCurrentHealth(50), m_HealthBar(*this, m_iCurrentHealth, m_iTotalHealth, 0.5f, { 255, 0, 0, 192 } ), m_fScaleFactor(0.5f)
{
	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/wolf.txt",
		"../Assets/sprites/wolf.png", "wolfspritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("wolfspritesheet");
	m_buildAnimations();
	// set frame width
	setWidth(40);

	// set frame height
	setHeight(40);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(GameObjectType::ENEMY);

	m_currentHeading = rand()% 360 + 1;
	m_changeDirection();
	//setVelocity(m_currentDirection * 10.0f);
	m_maxSpeed = 5.0f;
	m_currentDirection = glm::vec2(1.0f, 0.0f);
	m_turnRate = 5.0f;


	//setAcceleration(glm::vec2(0.1f, 0.0f));
	setState(SteeringState::SEEK);
	setTargetPosition({ 0, 0 });
	m_angleToTarget = 0.0f;
	m_feelerAngle = 30.0f; 
	m_feelerLength = 150.0f;
	m_distanceToTarget = 0.0f;
	m_arrivalRadius = 175.0f;
	m_arrivalTarget = 45.0f;
	m_turnFrame = 0;
	m_turnFrameMax = 3;
	m_avoidEndFrame = 0;
	m_avoidEndFrameMax = 21;
}

Enemy::~Enemy()
= default;

void Enemy::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch (m_currentAnimationState)
	{
	case WOLF_IDLE:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["idle"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_WALK:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["walk"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["run"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_RUN:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["run"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["run"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_BITE:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["bite"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["bite"].m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
	}

	m_HealthBar.draw();
}

void Enemy::update()
{
	m_checkSteeringState();
	m_checkBounds();
	move();

	m_HealthBar.update();
}

void Enemy::clean()
{

}

void Enemy::m_checkBounds()
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

glm::vec2 Enemy::getFeelerEndPosition(unsigned int feeler_number)
{
	// Centre feeler 
	glm::vec2 end = this->getPosition() + (m_currentDirection * m_feelerLength);
	
	// Left Feeler
	if (feeler_number == 0)
	{
		end = Util::rotateVectorLeft(end, m_feelerAngle);
	}

	// Right feeler
	else if (feeler_number == 2)
	{
		end = Util::rotateVectorRight(end, m_feelerAngle);
	}

	return end;
}

void Enemy::setFeeler(unsigned int feeler_number, bool value)
{
	m_feelerCol[feeler_number] = value;
}

void Enemy::setTargetPosition(glm::vec2 target_position)
{
	m_targetPosition = target_position;
}

void Enemy::move()
{
	setPosition(getPosition() + getVelocity());
	//setVelocity(getVelocity() * 0.95f);
	
}

void Enemy::accelerate()
{
	setAcceleration(glm::vec2{m_currentDirection.x * m_maxSpeed * 0.1, m_currentDirection.y * m_maxSpeed * 0.1});
	this->setVelocity(this->getVelocity() + this->getAcceleration());
	if (Util::magnitude(getVelocity()) > m_maxSpeed)
	{
		setVelocity(Util::normalize(getVelocity()) * m_maxSpeed);
	}
} 

void Enemy::decelerate()
{
	glm::vec2 newVel = this->getVelocity();
	newVel.x *= 0.9;
	newVel.y *= 0.9;
	this->setVelocity(newVel);
}

void Enemy::m_turn(float direction, bool to_target)
{
	if (to_target)
	{
		float angleDifference = abs(m_turnRate - m_angleToTarget);
		m_currentHeading += ((angleDifference < m_turnRate) ? m_angleToTarget * 0.2 : m_turnRate) * direction;
	}
	else
	{
		m_currentHeading += m_turnRate * direction;
	}
	m_changeDirection();
}

void Enemy::m_changeDirection()
{
	const auto x = cos( (90 + m_currentHeading) * Util::Deg2Rad);
	const auto y = sin( (90 + m_currentHeading) * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}

glm::vec2 Enemy::getTargetPosition()
{
	return m_targetPosition;
}

void Enemy::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-idle-1"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-idle-2"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-idle-3"));
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-idle-4"));

	m_pAnimations["idle"] = idleAnimation;

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-run-1"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-run-2"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-run-3"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-run-4"));
	runAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-run-5"));
	m_pAnimations["run"] = runAnimation;

	Animation walkAnimation = Animation();

	walkAnimation.name = "walk";
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-walk-1"));
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-walk-2"));
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-walk-3"));
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-walk-4"));

	m_pAnimations["walk"] = walkAnimation;

	Animation biteAnimation = Animation();

	biteAnimation.name = "bite";
	biteAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-bite-1"));
	biteAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-bite-2"));
	biteAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-bite-3"));
	biteAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-bite-4"));
	biteAnimation.frames.push_back(m_pSpriteSheet->getFrame("wolf-bite-5"));
	m_pAnimations["bite"] = biteAnimation;
}

void Enemy::m_checkSteeringState()
{
	switch (getState())
	{
	case SteeringState::IDLE:
		setVelocity({ 0, 0 });
		break;
	case SteeringState::SEEK:
		accelerate();
		m_seek();
		m_reorient();
		m_checkArrival();
		break;
	case SteeringState::ARRIVE:
		m_seek();
		m_reorient();
		m_arrive();
		m_checkArrival();
		break;
	case SteeringState::AVOID:
		m_seek();
		m_avoid();
		m_arrive();
		m_checkArrival();
		break;
	case SteeringState::FLEE:
		accelerate();
		m_flee();
		m_reorient();
		break;
	}
}

void Enemy::m_seek()
{
	glm::vec2 steeringVelocity;
	steeringVelocity = getTargetPosition() - getPosition();
	m_targetDirection = Util::normalize(steeringVelocity);
}

void Enemy::m_flee()
{
	glm::vec2 steeringVelocity;
	glm::vec2 nearCorner = { getPosition().x < getTargetPosition().x ? getWidth() * 0.75 : 800 - getWidth() * 0.75, getPosition().y < getTargetPosition().y ? getWidth() * 0.75 : 600 - getWidth() * 0.75 };
	float tx = (abs(nearCorner.x - getPosition().x) / abs(nearCorner.x - getTargetPosition().x));
	float ty = (abs(nearCorner.y - getPosition().y) / abs(nearCorner.y - getTargetPosition().y));
	steeringVelocity.x = Util::lerp(getPosition().x - getTargetPosition().x, nearCorner.x - getPosition().x, tx < ty ? 1 - tx : 1 - ty);
	steeringVelocity.y = Util::lerp(getPosition().y - getTargetPosition().y, nearCorner.y - getPosition().y, tx < ty ? 1 - tx : 1 - ty);
	m_targetDirection = Util::normalize(steeringVelocity);
}

void Enemy::m_checkArrival()
{
	m_distanceToTarget = Util::distance(getPosition(), getTargetPosition());
	if (m_distanceToTarget <= m_arrivalTarget)
	{
		this->setState(IDLE);
	}
	else if (m_distanceToTarget <= m_arrivalRadius)
	{
		this->setState(ARRIVE);
	}
}

void Enemy::m_checkCollisions()
{
}

void Enemy::m_arrive()
{
	if (m_distanceToTarget <= m_arrivalRadius)
	{
		decelerate();
	}
	else
	{
		accelerate();
	}
}

void Enemy::m_avoid()
{
	if (m_avoidDirection < 0)
	{
		m_turn(1.0f);
	}
	else if (m_avoidDirection > 0)
	{
		m_turn(-1.0f);
	}
}

void Enemy::m_reorient()
{
	if (m_feelerCol[0] == false && m_feelerCol[1] == false && m_feelerCol[2] == false)
	{
		m_angleToTarget = Util::signedAngle(m_currentDirection, m_targetDirection);
		if (m_angleToTarget > 0.0f)
			m_turn(1.0f);
		else if (m_angleToTarget < 0.0f)
			m_turn(-1.0f);
	}
	else if (m_feelerCol[0] == true || m_feelerCol[1] == true || m_feelerCol[2] == true)
	{
		m_avoidEndFrame = 0;

		if ((m_feelerCol[0] == true && m_feelerCol[1] == false && m_feelerCol[2] == false) ||
			(m_feelerCol[0] == false && m_feelerCol[1] == true && m_feelerCol[2] == false) ||
			(m_feelerCol[0] == true && m_feelerCol[1] == true && m_feelerCol[2] == false) ||
			(m_feelerCol[0] == true && m_feelerCol[1] == false && m_feelerCol[2] == true) ||
			(m_feelerCol[0] == true && m_feelerCol[1] == true && m_feelerCol[2] == true))
		{
			m_turn(-1.0, false);
		}
		else if ((m_feelerCol[0] == false && m_feelerCol[1] == false && m_feelerCol[2] == true) ||
			(m_feelerCol[0] == false && m_feelerCol[1] == true && m_feelerCol[2] == true))
		{
			m_turn(1.0, false);
		}
	}
}