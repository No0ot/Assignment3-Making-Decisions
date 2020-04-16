#include "Ranged_Enemy.h"
#include "TextureManager.h"
#include "Game.h"

Ranged_Enemy::Ranged_Enemy()
{
	m_iTotalHealth = 70;
	m_iCurrentHealth = 70;
	m_HealthBar = new HealthBar(*this, m_iCurrentHealth, m_iTotalHealth, 0.5f, { 255, 0, 0, 192 });
	m_fScaleFactor = 2.0f;

	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/mech-Sheet.txt",
		"../Assets/sprites/mech-Sheet.png", "mechspritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("mechspritesheet");
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

	m_currentHeading = rand() % 360 + 1;
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
	m_arrivalTarget = 40.0f;
	m_avoidEndFrame = 0;
	m_avoidEndFrameMax = 10;
	m_numFramesAvoiding = 0;

	m_smellRadius = 100.0f;
	m_fFOV = 40;

	m_iDamage = -10;
	m_iPtsValue = 50;
}

void Ranged_Enemy::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch (m_currentAnimationState)
	{
	case WOLF_IDLE:
		TheTextureManager::Instance()->playAnimation("mechspritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["idle"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_RUN:
		TheTextureManager::Instance()->playAnimation("mechspritesheet", m_pAnimations["walk"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["walk"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_BITE:
		TheTextureManager::Instance()->playAnimation("mechspritesheet", m_pAnimations["shoot"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["shoot"].m_currentFrame, 0.5f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	}

	m_HealthBar->draw();

	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, -m_feelerAngle));
	Util::DrawLine(getPosition(), getPosition() + m_currentDirection * m_feelerLength);
	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, m_feelerAngle));
	Util::DrawCircle(getPosition(), getHeight() * m_fScaleFactor);
	Util::DrawCircle(getPosition(), getSmellRadius());
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

}

void Ranged_Enemy::update()
{
	m_checkHealth();
	m_checkBehaviourState();
	m_checkSteeringState();
	m_checkBounds();
	m_HealthBar->update();

	std::cout << "STEERINGSTATE: " << getState() << std::endl;
	std::cout << "BEHAVIOURSTATE: " << (int)getBehaviour() << std::endl;
	std::cout << "TARGETPOSITION: " << getTargetPosition().x << " " << getTargetPosition().y << std::endl;

}