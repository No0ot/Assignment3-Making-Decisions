#include "Ranged_Enemy.h"
#include "TextureManager.h"
#include "Game.h"

Ranged_Enemy::Ranged_Enemy()
{
	canShoot = true;
	playerDistance = 0;
	m_iTotalHealth = 80;
	m_iCurrentHealth = 80;
	m_HealthBar = new HealthBar(*this, m_iCurrentHealth, m_iTotalHealth, 0.5f, { 255, 0, 0, 192 });
	m_fScaleFactor = 2.0f;
	for (int i = 0; i < 3; i++)
		quarterHealth[i] = true;

	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/mech.txt",
		"../Assets/sprites/mech.png", "mech", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("mech");
	m_buildAnimations();
	m_meleeCollisionBox = new Collider;
	m_meleeCollisionBox->setPosition(glm::vec2{ 500,1000 });
	m_meleeCollisionBox->setHeight(50);
	m_meleeCollisionBox->setWidth(50);
	// set frame width
	setWidth(40);
	setAnimState(WOLF_IDLE);
	// set frame height
	setHeight(40);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(GameObjectType::RANGED_ENEMY);
	m_shootFrame = 0;
	m_shootFrameMax = 120;

	m_currentHeading = 90;
	//setVelocity(m_currentDirection * 10.0f);
	m_maxSpeed = 3.0f;
	m_currentDirection = glm::vec2(1.0f, 0.0f);
	m_turnRate = 3.0f;
	setBehaviour(BehaviourState::PATROL);
	//setAcceleration(glm::vec2(0.1f, 0.0f));
	setState(SteeringState::SEEK);
	setTargetPosition({ 0, 0 });
	m_angleToTarget = 0.0f;
	m_feelerAngle = 30.0f;
	m_feelerLength = 50.0f;
	m_distanceToTarget = 0.0f;
	m_arrivalRadius = 100.0f;
	m_arrivalTarget = 10.0f;
	m_avoidEndFrame = 0;
	m_avoidEndFrameMax = 10;
	m_numFramesAvoiding = 0;

	m_smellRadius = 100.0f;
	m_fFOV = 40;

	m_iDamage = -10;
	m_iPtsValue = 50;

	bulletspawnPos = glm::vec2(getPosition().x + 24, getPosition().y + 16);
	directionvector = { bulletspawnPos.x - getPosition().x, bulletspawnPos.y - getPosition().y };
	mag = Util::magnitude(directionvector);
}

void Ranged_Enemy::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch (m_currentAnimationState)
	{
	case WOLF_IDLE:
		TheTextureManager::Instance()->playAnimation("mech", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["idle"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading + 180, 255, true);
		break;
	case WOLF_WALK:
		TheTextureManager::Instance()->playAnimation("mech", m_pAnimations["walk"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["walk"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading + 180, 255, true);
		break;
	case WOLF_RUN:
		TheTextureManager::Instance()->playAnimation("mech", m_pAnimations["walk"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["walk"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading + 180, 255, true);
		break;
	case WOLF_BITE:
		TheTextureManager::Instance()->playAnimation("mech", m_pAnimations["shoot"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["shoot"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading + 180, 255, true);
		break;
	}

	for (int i = 0; i < m_pBulletvec.size(); i++)
	{
		if (m_pBulletvec[i] != nullptr)
			m_pBulletvec[i]->draw();
	}

	m_HealthBar->draw();
	glm::vec2 directionvector = bulletspawnPos - getPosition();
	float mag = Util::magnitude(directionvector);
	glm::vec2 dv2 = Util::normalize(directionvector);
	Util::DrawLine(getPosition(), getPosition() + dv2 * mag);
	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, -m_feelerAngle));
	Util::DrawLine(getPosition(), getPosition() + m_currentDirection * m_feelerLength);
	Util::DrawLine(getPosition(), getTargetPosition());
	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, m_feelerAngle));
	Util::DrawCircle(getPosition(), getHeight());
	Util::DrawCircle(getPosition(), 350);
}

void Ranged_Enemy::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Walk-2"));

	m_pAnimations["idle"] = idleAnimation;

	Animation walkAnimation = Animation();

	walkAnimation.name = "walk";
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Walk-1"));
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Walk-2"));
	walkAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Walk-3"));

	m_pAnimations["walk"] = walkAnimation;

	Animation shootAnimation = Animation();

	shootAnimation.name = "shoot";
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Shoot-1"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Shoot-2"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Shoot-3"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Shoot-4"));
	shootAnimation.frames.push_back(m_pSpriteSheet->getFrame("Mech-Shoot-5"));

	m_pAnimations["shoot"] = shootAnimation;

}

void Ranged_Enemy::m_attack()
{
	m_targetPosition = m_playersPos;
	if (playerDistance <= 300)
	{
		setAnimState(WOLF_RUN);
		setState(FLEE);
	}
	else
		setState(SEEK);
	m_maxSpeed = 1.5f;
	if (hasLOS() && playerDistance >= 250 && playerDistance <= 300)
	{	
		setState(IDLE);
		m_maxSpeed = 0.0f;
		if (m_shootFrame >= m_shootFrameMax)
		{
			setAnimState(WOLF_BITE);
			canShoot = false;
			m_pBulletvec.push_back(new Bullet(bulletspawnPos, m_currentHeading + 90, m_iDamage));
			m_shootFrame = 0;
		}
		m_shootFrame++;
		//setAnimState(WOLF_IDLE);
		canShoot = true;
		//setBehaviour(BehaviourState::IDLE2);
	}
}

void Ranged_Enemy::m_idle()
{
	setState(IDLE);
	setAnimState(WOLF_IDLE);
	//m_Stateframes = 0;
	m_StateframesMax = 120;
	if (m_Stateframes >= m_StateframesMax)
	{
		m_Stateframes = 0;
		setBehaviour(BehaviourState::PATROL);
	}
	m_Stateframes++;

}

void Ranged_Enemy::m_patrol()
{
	m_targetPosition = m_patrolPoint;
	setState(SEEK);
	setAnimState(WOLF_WALK);
	m_maxSpeed = 1.2f;
	if (m_arrived)
	{
		m_specialnumber = rand() % 4;
	}
	if (canDetect())
		setBehaviour(BehaviourState::ATTACK);
}

void Ranged_Enemy::m_cower()
{
	m_targetPosition = m_nearestCoverTile;
	setState(SEEK);
	m_maxSpeed = 1.2f;
	if (m_arrived)
	{
		setBehaviour(BehaviourState::IDLE2);
	}

}

void Ranged_Enemy::m_checkHealth()
{

	if (m_iCurrentHealth <= 60 && quarterHealth[0])
	{
		setBehaviour(BehaviourState::COWER);
		quarterHealth[0] = false;
	}
	if (m_iCurrentHealth <= 40 && quarterHealth[1])
	{
		setBehaviour(BehaviourState::COWER);
		quarterHealth[1] = false;
	}
	if (m_iCurrentHealth <= 20 && quarterHealth[2])
	{
		setBehaviour(BehaviourState::COWER);
		quarterHealth[2] = false;
	}
}

void Ranged_Enemy::updatebulletspawn()
{
	glm::vec2 tempos2 = Util::rotateVector(m_currentDirection, 24.785);
	tempos2 = Util::normalize(tempos2);
	bulletspawnPos = { getPosition().x + (tempos2.x * mag), getPosition().y + (tempos2.y * mag) };

}


void Ranged_Enemy::update()
{
	playerDistance = Util::distance(getPosition(), m_playersPos);
	m_checkHealth();
	updatebulletspawn();
	m_checkBehaviourState();
	m_checkSteeringState();
	m_checkBounds();
	m_HealthBar->update();
		if (m_pAnimations["shoot"].m_currentFrame == (int)m_pAnimations["shoot"].frames.size() - 1)
		{
			m_pAnimations["shoot"].m_currentFrame = 0;
			setAnimState(WOLF_IDLE);
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
	std::cout << "STEERINGSTATE: " << getState() << std::endl;
	std::cout << "BEHAVIOURSTATE: " << (int)getBehaviour() << std::endl;
	std::cout << "TARGETPOSITION: " << getTargetPosition().x << " " << getTargetPosition().y << std::endl;

}