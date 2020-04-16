#include "Melee_Enemy.h"
#include "TextureManager.h"
#include "Game.h"

Melee_Enemy::Melee_Enemy()
{
	
	m_iTotalHealth = 50;
	m_iCurrentHealth = 50;
	m_HealthBar = new HealthBar(*this, m_iCurrentHealth, m_iTotalHealth, 0.5f, { 255, 0, 0, 192 });
	m_fScaleFactor = 1.2f;
	canCower = true;

	TheTextureManager::Instance()->loadSpriteSheet("../Assets/sprites/wolf.txt",
		"../Assets/sprites/wolf.png", "wolfspritesheet", TheGame::Instance()->getRenderer());

	m_pSpriteSheet = TheTextureManager::Instance()->getSpriteSheet("wolfspritesheet");
	m_meleeCollisionBox = new Collider;
	m_buildAnimations();
	// set frame width
	setWidth(40);
	setAnimState(WOLF_IDLE);
	// set frame height
	setHeight(40);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(GameObjectType::MELEE_ENEMY);
	m_meleeCollisionBox->setPosition(glm::vec2{ 500,1000 });
	m_meleeCollisionBox->setHeight(50);
	m_meleeCollisionBox->setWidth(50);

	m_currentHeading = rand() % 360 + 1;
	//setVelocity(m_currentDirection * 10.0f);
	
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
	m_maxFramesAvoiding = 30;

	m_smellRadius = 100.0f;
	m_fFOV = 40;

	m_iDamage = -10;
	m_iPtsValue = 25;
}

void Melee_Enemy::draw()
{
	const int xComponent = getPosition().x;
	const int yComponent = getPosition().y;

	switch (m_currentAnimationState)
	{
	case WOLF_IDLE:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["idle"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["idle"].m_currentFrame, 0.2f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		
		break;
	case WOLF_WALK:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["walk"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["run"].m_currentFrame, 0.25f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_RUN:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["run"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["run"].m_currentFrame, 0.2f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	case WOLF_BITE:
		TheTextureManager::Instance()->playAnimation("wolfspritesheet", m_pAnimations["bite"],
			getPosition().x, getPosition().y, m_fScaleFactor, m_pAnimations["bite"].m_currentFrame, 0.12f,
			TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
		break;
	}

	m_HealthBar->draw();

	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, -m_feelerAngle));
	Util::DrawLine(getPosition(), getPosition() + m_currentDirection * m_feelerLength);
	Util::DrawLine(getPosition(), getTargetPosition());
	Util::DrawLine(getPosition(), getPosition() + Util::rotateVector(m_currentDirection * m_feelerLength, m_feelerAngle));
	Util::DrawCircle(getPosition(), getHeight() * m_fScaleFactor);
	Util::DrawCircle(getPosition(), getSmellRadius());
	Util::DrawCircle(m_meleeCollisionBox->getPosition(), m_meleeCollisionBox->getWidth());
}

void Melee_Enemy::m_buildAnimations()
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

void Melee_Enemy::m_attack()
{
	setState(SEEK);
	m_maxSpeed = 3.0f;
	if (m_arrived)
	{
		setAnimState(WOLF_BITE);
		glm::vec2 tempos = Util::rotateVector(m_currentDirection, 0);
		tempos = Util::normalize(tempos);
		m_meleeCollisionBox->setPosition(glm::vec2{ getPosition().x + (tempos.x * 50.0f), getPosition().y + (tempos.y * 50.0f) });
		//m_bite();
		setBehaviour(BehaviourState::IDLE2);
	}
	
}

void Melee_Enemy::update()
{
	m_checkHealth();
	m_checkBehaviourState();
	m_checkSteeringState();
	m_checkBounds();
	m_HealthBar->update();
	switch(getAnimState())
	{
	case WOLF_BITE:
		m_meleeCollisionBox->setPosition(glm::vec2{ getPosition().x + (m_currentDirection.x * 50.0f), getPosition().y + (m_currentDirection.y * 50.0f) });
		break;
	case WOLF_IDLE:
		m_meleeCollisionBox->setPosition(glm::vec2{ 500,1000 });
		break;
	case WOLF_RUN:
		m_meleeCollisionBox->setPosition(glm::vec2{ 500,1000 });

		break;
	}

	/*std::cout << "STEERINGSTATE: " << getState() << std::endl;
	std::cout << "BEHAVIOURSTATE: " << (int)getBehaviour() << std::endl;
	std::cout << "TARGETPOSITION: " << getTargetPosition().x << " " << getTargetPosition().y << std::endl;*/

}

