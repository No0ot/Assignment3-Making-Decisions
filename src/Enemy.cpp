#include "Enemy.h"
#include "Game.h"

Enemy::Enemy() : m_Stateframes(0), m_arrived(false), canDamage(true) {}

Enemy::~Enemy()
= default;

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

float Enemy::getFOV() const
{
	return m_fFOV;
}

glm::vec2 Enemy::getDirection() const
{
	return m_currentDirection;
}

int Enemy::getDamage()
{
	return m_iDamage;
}

bool Enemy::changeHealth(int change)
{
	m_iCurrentHealth += change;
	if (m_iCurrentHealth <= 0)
	{
		return true;
	}
	return false;
}

int Enemy::getPtsValue()
{
	return m_iPtsValue;
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
	//std::cout << "m_turn(" << angle << ") called!" << std::endl;
	m_currentHeading += angle;
	const auto x = cos((90 + m_currentHeading) * Util::Deg2Rad);
	const auto y = sin((90 + m_currentHeading) * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}

glm::vec2 Enemy::getTargetPosition()
{
	return m_targetPosition;
}



BehaviourState Enemy::getBehaviour()
{
	return m_Behaviour;
}

void Enemy::setBehaviour(BehaviourState state)
{
	m_Behaviour = state;
}

void Enemy::setNextPatrolPoint(glm::vec2 target_position)
{
	m_nextPatrolPoint = target_position;
}

void Enemy::m_checkBehaviourState()
{
	switch (getBehaviour())
	{
	case BehaviourState::IDLE2:
		m_idle();
		break;
	case BehaviourState::PATROL:
		m_patrol();
		break;
	case BehaviourState::ATTACK:
		m_attack();
		break;
	case BehaviourState::COWER:
		m_cower();
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
		m_arrived = true;
			randomnum = rand() % 4;

	}
	else if (m_distanceToTarget <= m_arrivalRadius)
	{
		this->setState(ARRIVE);

	}
	else
	{
		m_arrived = false;
		//this->setState(SEEK);
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
			m_turn(m_turnRate);
			
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

WolfAnimationState Enemy::getAnimState()
{
	return m_currentAnimationState;
}

void Enemy::setAnimState(WolfAnimationState state)
{
	m_currentAnimationState = state;
}