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
	//setVelocity(m_currentDirection * 10.0f);
	m_maxSpeed = 3.0f;
	m_currentDirection = glm::vec2(1.0f, 0.0f);
	m_turnRate = 3.0f;

	//setAcceleration(glm::vec2(0.1f, 0.0f));
	setBehaviour(BehaviourState::PATROL);
	//setState(SteeringState::SEEK);
	setTargetPosition({ 0, 0 });
	m_angleToTarget = 0.0f;
	m_feelerAngle = 30.0f; 
	m_feelerLength = 50.0f;
	m_distanceToTarget = 0.0f;
	m_arrivalRadius = 100.0f;
	m_arrivalTarget = 40.0f;
	m_avoidEndFrame = 0;
	m_avoidEndFrameMax = 10;
	m_numFramesAvoiding = 0;

	m_smellRadius = 150.0f;
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

	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, -m_feelerAngle));
	Util::DrawLine(getPosition(), getPosition() + m_currentDirection * m_feelerLength);
	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, m_feelerAngle));
	Util::DrawCircle(getPosition(), getHeight() * m_fScaleFactor);
	Util::DrawCircle(getPosition(), getSmellRadius());
}

void Enemy::update()
{
	m_checkBehaviourState();
	m_checkSteeringState();
	m_checkBounds();

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
	glm::vec2 ret = { 0, 0 };

	switch (feeler_number)
	{
	case 0:
		ret = getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, m_feelerAngle);
		break;
	case 1:
		ret = getPosition() + m_currentDirection * m_feelerLength;
		break;
	case 2:
		ret = getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, -m_feelerAngle);
		break;
	}

	return ret;
}

void Enemy::setFeeler(unsigned int feeler_number, bool value)
{
	//std::cout << feeler_number << " set to " << (value == false ? "false" : "true") << std::endl;
	m_feelerCol[feeler_number] = value;
}

void Enemy::setTargetPosition(glm::vec2 target_position)
{
	m_targetPosition = target_position;
}

void Enemy::setLOS(bool value)
{
	m_hasLOS = value;
}

bool Enemy::hasLOS() const
{
	return m_hasLOS;
}

float Enemy::getSmellRadius()
{
	return m_smellRadius;
}

void Enemy::setSmell(bool value)
{
	m_hasSmell = value;
}

bool Enemy::hasSmell() const
{
	return m_hasSmell;
}

bool Enemy::canDetect() const
{
	if (hasLOS() || hasSmell())
	{
		return true;
	}
	return false;
}

void Enemy::move()
{
	setVelocity(m_currentDirection * m_maxSpeed);
	setPosition(getPosition() + getVelocity());
	//setVelocity(getVelocity() * 0.95f);
	
}

//void Enemy::accelerate()
//{
//	setAcceleration(glm::vec2{m_currentDirection.x * m_maxSpeed * 0.1, m_currentDirection.y * m_maxSpeed * 0.1});
//	this->setVelocity(this->getVelocity() + this->getAcceleration());
//	if (Util::magnitude(getVelocity()) > m_maxSpeed)
//	{
//		setVelocity(Util::normalize(getVelocity()) *  m_maxSpeed);
//	}
//} 
//
//void Enemy::decelerate()
//{
//	glm::vec2 newVel = this->getVelocity();
//	newVel.x *= 0.9;
//	newVel.y *= 0.9;
//	this->setVelocity(newVel);
//}

void Enemy::m_turn(float angle)
{
	std::cout << "m_turn(" << angle << ") called!" << std::endl;
	m_currentHeading += angle;
	const auto x = cos((90 + m_currentHeading) * Util::Deg2Rad);
	const auto y = sin((90 + m_currentHeading) * Util::Deg2Rad);
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

BehaviourState Enemy::getBehaviour()
{
	return m_Behaviour;
}

void Enemy::setBehaviour(BehaviourState state)
{
	m_Behaviour = state;
}

void Enemy::m_checkBehaviourState()
{
	switch (getBehaviour())
	{
	case BehaviourState::IDLE2:
		//set target in level1Scene
		setState(IDLE);
		// wait 5 seconds than setBehaviour to PATROL
		break;
	case BehaviourState::PATROL:
		//set target in level1Scene
		setState(SEEK);
		if (hasSmell() || hasLOS())
			setBehaviour(BehaviourState::ASSAULT);
		break;
	case BehaviourState::ATTACK:
		//execute a command to attack once fininsihed set behaviour to FLEE or ASSAULT again
		break;
	case BehaviourState::ASSAULT:
		//set target in level1Scene
		setState(SEEK);
		//setState(ATTACK) once arrived
		break;
	case BehaviourState::FLEE:
		//set target in level1Scene
		setState(FLEE);
		break;
	case BehaviourState::COWER:
		//set target in level1Scene
		setState(SEEK);
		//setState(IDLE) once arrived
		//wait 5 seconds then set behaviour to patrol
		break;
	}
}


void Enemy::m_checkSteeringState()
{
	if (m_checkFeelers())
	{

	}
	else
	{
		switch (getState())
		{
		case SteeringState::IDLE:
			//decelerate();
			//move();
			break;
		case SteeringState::SEEK:
			//std::cout << "Seeking! x: " << getVelocity().x << " y: " << getVelocity().y << std::endl;
			//accelerate();
			m_seek();
			m_reorient();
			m_checkArrival();
			move();
			break;
		case SteeringState::ARRIVE:
			//std::cout << "Arriving!" << std::endl;
			m_seek();
			m_reorient();
			//m_arrive();
			m_checkArrival();
			move();
			break;
		case SteeringState::AVOID:
			std::cout << "IN AVOID STATE! BAD!" << std::endl;
			break;
		case SteeringState::FLEE:
			//std::cout << "Fleeing!" << std::endl;
			//accelerate();
			m_flee();
			m_reorient();
			move();
			break;
		}
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
	steeringVelocity = getPosition() - getTargetPosition();
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
	else
	{
		this->setState(SEEK);
	}
}

bool Enemy::m_checkFeelers()
{
	if (m_feelerCol[0] == false && m_feelerCol[1] == false && m_feelerCol[2] == false) // If none of the feelers are feeling anything (this is determined in the game scene collisions function)
	{
		if (m_avoidEndFrame >= m_avoidEndFrameMax)
		{
			m_numFramesAvoiding = 0;
			return false;
		}
		else
		{
			m_avoidEndFrame++;
			m_numFramesAvoiding++;
			if (m_avoidDirection == 0.0f)
			{
				m_turn(m_turnRate);
			}
			else
			{
				m_turn(m_turnRate * m_avoidDirection);
			}
		}
	}
	else
	{
		m_avoidEndFrame = 0;
		if (m_numFramesAvoiding >= m_maxFramesAvoiding)
		{
			std::cout << "Argh! I can't find a way past this!" << std::endl;
			m_turn(m_turnRate * 0.5);
		}
		else
		{
			m_numFramesAvoiding++;

			if ((m_feelerCol[0] == true && m_feelerCol[1] == false && m_feelerCol[2] == false) ||
				(m_feelerCol[0] == false && m_feelerCol[1] == true && m_feelerCol[2] == false) ||
				(m_feelerCol[0] == true && m_feelerCol[1] == true && m_feelerCol[2] == false) ||
				(m_feelerCol[0] == true && m_feelerCol[1] == false && m_feelerCol[2] == true) ||
				(m_feelerCol[0] == true && m_feelerCol[1] == true && m_feelerCol[2] == true))
			{
				m_avoidDirection = -1.0;
				m_turn(m_turnRate * m_avoidDirection);
			}
			else if ((m_feelerCol[0] == false && m_feelerCol[1] == false && m_feelerCol[2] == true) ||
				(m_feelerCol[0] == false && m_feelerCol[1] == true && m_feelerCol[2] == true))
			{
				m_avoidDirection = 1.0;
				m_turn(m_turnRate * m_avoidDirection);
			}
		}
	}
	return true;
}

void Enemy::m_arrive()
{
	//if (m_distanceToTarget <= m_arrivalRadius)
	//{
	//	decelerate();
	//}
	//else
	//{
	//	accelerate();
	//}
}

void Enemy::m_reorient()
{
	m_angleToTarget = Util::signedAngle(m_currentDirection, m_targetDirection);
	if (m_angleToTarget > 0.0f)
	{
		m_turn(m_angleToTarget < m_turnRate ? m_angleToTarget : m_turnRate);
	}
	else if (m_angleToTarget < 0.0f)
	{
		m_turn(m_angleToTarget > -m_turnRate ? m_angleToTarget : -m_turnRate);
	}
}