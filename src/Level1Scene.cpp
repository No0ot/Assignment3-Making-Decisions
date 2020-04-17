#include "Level1Scene.h"
#include "Game.h"
#include "ExplosionManager.h"
#include "Util.h"

Level1Scene::Level1Scene() :
	m_iCurrentPts(0), m_iTotalPts(100), m_PtsBar(10, 10, m_iCurrentPts, m_iTotalPts, 2.0f, { 192, 192, 255, 192 }),
	m_iRespawnFrame(100), m_iRespawnFrameMax(100), m_RespawnBar(300, 300, m_iRespawnFrame, m_iRespawnFrameMax, 2.0f, { 255, 128, 128, 192 }),
	m_iScreenMsgFrame(0), m_iScreenMsgFrameMax(100)
{
	Level1Scene::start();
	TheSoundManager::Instance()->load("../Assets/audio/EnemyHit.wav", "EnemyHit", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/EnemyMeleed.wav", "EnemyMeleed", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/EnemyDie.wav", "EnemyDie", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/EnemyShoot", "EnemyShoot", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/ObstacleHit.wav", "ObstacleBumped", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/ObstacleShot.wav", "ObstacleShot", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/PlayerStep.wav", "PlayerStep", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/PlayerHurt.ogg", "PlayerHit", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/PlayerMelee.wav", "PlayerMelee", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/PlayerShoot.wav", "PlayerShoot", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/PlayerStep.wav", "PlayerStep", sound_type::SOUND_SFX);




}

Level1Scene::~Level1Scene()
= default;

void Level1Scene::draw()
{
	drawDisplayList();
	m_ScreenMsg->draw();

	if (m_iRespawnFrame < m_iRespawnFrameMax)
	{
		m_RespawnBar.draw();
	}

	m_PtsBar.draw();
}

void Level1Scene::update()
{
	if (m_iScreenMsgFrame >= m_iScreenMsgFrameMax)
	{
		Uint8 alpha = m_ScreenMsg->getAlpha();
		if (alpha > 0)
		{
			m_ScreenMsg->setAlpha(alpha - 1);
		}
	}
	else
	{
		m_iScreenMsgFrame++;
	}

	updateDisplayList();
	m_pPlayer->update();
	for (unsigned int j = 0; j < m_pEnemyVec.size(); j++)
	{
		m_pEnemyVec[j]->update();
	}
	updateEnemyTargets();
	m_checkCollisions();

	m_PtsBar.update();
	m_RespawnBar.update();

	if (m_pEnemyVec.size() == 0)
	{
		if (m_iRespawnFrame >= m_iRespawnFrameMax)
		{
			int temp = rand() % 2;
			if (temp == 0)
			{
				m_pEnemyVec.push_back(new Ranged_Enemy(m_RangedLoadouts[rand() % 3]));
				addChild(m_pEnemyVec.back());
				m_pEnemyVec.back()->DisplayListIndexInScene = numberOfChildren() - 1;
			}
			else if (temp == 1)
			{
				m_pEnemyVec.push_back(new Melee_Enemy(m_MeleeLoadouts[rand() % 3]));
				addChild(m_pEnemyVec.back());
				m_pEnemyVec.back()->DisplayListIndexInScene = numberOfChildren() - 1;
			}
		}
		else
		{
			m_iRespawnFrame++;
		}
	}
}

void Level1Scene::clean()
{

	removeAllChildren();
}

void Level1Scene::handleEvents()
{
	auto wheel = 0;

	SDL_Keycode keyPressed;
	SDL_Keycode keyReleased;

	bool pControlState = m_pPlayer->getControlState();
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			TheGame::Instance()->setMousePosition(m_mousePosition);
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
			m_pPlayer->setPosition(glm::vec2{ m_mousePosition.x, m_mousePosition.y });
			}
			break;
		case SDL_KEYDOWN:
			keyPressed = event.key.keysym.sym;
			switch (keyPressed)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_1:
				//TheGame::Instance()->changeSceneState(SceneState::PLAY_SCENE);
				break;
			case SDLK_2:
				//TheGame::Instance()->changeSceneState(SceneState::END_SCENE);
				break;
			}
			// movement keys
			{
				if (keyPressed == SDLK_f && m_pPlayer->canMelee == true)
				{
					m_pPlayer->canShoot = true;
					TheSoundManager::Instance()->playSound("PlayerMelee", 0);
					m_pPlayer->melee();
				}
				
				if (keyPressed == SDLK_BACKQUOTE)
				{
					TheGame::Instance()->toggleDebugMode();
					std::cout << "Debug mode " << (TheGame::Instance()->getDebugMode() == true ? "activated" : "deactivated") << "." << std::endl;
				}

				if (keyPressed == SDLK_w)
				{
					//m_pPlayer->canMelee = true;
					//m_pPlayer->canShoot = true;
					std::cout << "move forward" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_RUN);
					TheSoundManager::Instance()->playSound("PlayerStep", 0);
					m_pPlayer->moveForward();
					
				}

				if (keyPressed == SDLK_a && pControlState == false)
				{
					//m_pPlayer->canShoot = true;
					//m_pPlayer->canMelee = true;
					//std::cout << "move left" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_IDLE);
					m_pPlayer->turnLeft();
					
				}

				if (keyPressed == SDLK_s )
				{
					//m_pPlayer->canShoot = true;
					//m_pPlayer->canMelee = true;
					std::cout << "move back" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_RUN);
					m_pPlayer->moveBack();
					
				}

				if (keyPressed == SDLK_d && pControlState == false)
				{
					//m_pPlayer->canShoot = true;
					//m_pPlayer->canMelee = true;
					//std::cout << "move right" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_IDLE);
					m_pPlayer->turnRight();
					
				}


				if (keyPressed == SDLK_SPACE && m_pPlayer->canShoot)
				{
					//m_pPlayer->canMelee = true;
					m_pPlayer->setAnimationState(PLAYER_SHOOT);
					TheSoundManager::Instance()->playSound("PlayerShoot", 0);
					m_pPlayer->shoot();
					

				}

				if (keyPressed == SDLK_c && pControlState == false)
				{
					//m_pPlayer->canMelee = true;
					//m_pPlayer->canShoot = true;
					m_pPlayer->turnaround();
					
				}
				if (keyPressed == SDLK_x)
					TheGame::Instance()->changeSceneState(SceneState::START_SCENE);
				
			}
			
			break;
		case SDL_KEYUP:
			keyReleased = event.key.keysym.sym;

			if (keyReleased == SDLK_w)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE);
			}

			if (keyReleased == SDLK_s)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE);
			}

		
			break;
			
		}
	}
}

void Level1Scene::start()
{
	m_MeleeLoadouts[0] = Melee_Enemy::Loadout({ 255, 255, 255, 255 }, 1.2f, 50, -10, 100.0f, 40.0f, 10);
	m_MeleeLoadouts[1] = Melee_Enemy::Loadout({ 128, 128, 255, 255 }, 1.4f, 75, -15, 100.0f, 40.0f, 20);
	m_MeleeLoadouts[2] = Melee_Enemy::Loadout({ 128, 255, 128, 255 }, 1.0f, 40, -15, 100.0f, 80.0f, 10);
	m_MeleeLoadouts[3] = Melee_Enemy::Loadout({ 255, 128, 128, 255 }, 1.6f, 60, -15, 200.0f, 60.0f, 25);
	m_RangedLoadouts[0] = Ranged_Enemy::Loadout({ 255, 255, 255, 255 }, 2.0, 80, -10, 100.0f, 40.0f, 20);
	m_RangedLoadouts[1] = Ranged_Enemy::Loadout({ 128, 128, 255, 255 }, 2.3, 100, -15, 150.0f, 40.0f, 30);
	m_RangedLoadouts[2] = Ranged_Enemy::Loadout({ 128, 255, 128, 255 }, 1.7, 50, -10, 100.0f, 80.0f, 15);
	m_RangedLoadouts[3] = Ranged_Enemy::Loadout({ 255, 128, 128, 255 }, 2.6, 90, -30, 100.0f, 30.0f, 40);

	m_ScreenMsg = new Label("GAME START!", "Consolas", 30, { 192, 192, 255, 255 }, glm::vec2(400.0f, 250.0f));
	m_ScreenMsg->setParent(this);
	addChild(m_ScreenMsg);

	m_buildGrid();
	m_mapTiles();
	for (int i = 0; i < 10; i++)
	{
		m_pObstacleVec.push_back(new Obstacle());
		addChild(m_pObstacleVec.back());

	}
	m_spawnObstacles();
	for (int i = 0; i < 1; i++)
	{
		int temp = rand() % 2;
		if (temp == 0)
		{
			m_pEnemyVec.push_back(new Ranged_Enemy(m_RangedLoadouts[rand() % 3]));
			addChild(m_pEnemyVec.back());
			m_pEnemyVec.back()->DisplayListIndexInScene = numberOfChildren() - 1;
		}
		else if (temp == 1)
		{
			m_pEnemyVec.push_back(new Melee_Enemy(m_MeleeLoadouts[rand() % 3]));
			addChild(m_pEnemyVec.back());
			m_pEnemyVec.back()->DisplayListIndexInScene = numberOfChildren() - 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		m_pPatrolPoints.push_back(new PatrolPoint());
		m_spawnObject(m_pPatrolPoints.back());
		m_pPatrolPoints.back()->getTile()->setTileState(GOAL);
	}

	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_spawnObject(m_pPlayer);

	m_spawnEnemy();
}

void Level1Scene::m_buildGrid()
{
	const auto size = Config::TILE_SIZE;
	const auto offset = size * 0.5f;

	m_pGrid = std::vector<Tile*>(); // instantiates a structure of type vector<Tile*>

	for (auto row = 0; row < Config::ROW_NUM; ++row)
	{
		for (auto col = 0; col < Config::COL_NUM; ++col)
		{
			auto tile = new Tile(glm::vec2(offset + size * col, offset + size * row),
				glm::vec2(col, row));
			addChild(tile);
			tile->setTileState(UNVISITED);

			m_pGrid.push_back(tile);
		}
	}
}

void Level1Scene::m_mapTiles()
{
	for (auto tile : m_pGrid)
	{
		const auto x = tile->getGridPosition().x;
		const auto y = tile->getGridPosition().y;

		if (y != 0) { tile->setUp(m_pGrid[x + ((y - 1) * Config::COL_NUM)]); }
		if (x != Config::COL_NUM - 1) { tile->setRight(m_pGrid[(x + 1) + (y * Config::COL_NUM)]); }
		if (y != Config::ROW_NUM - 1) { tile->setDown(m_pGrid[x + ((y + 1) * Config::COL_NUM)]); }
		if (x != 0) { tile->setLeft(m_pGrid[(x - 1) + (y * Config::COL_NUM)]); }

	}
}

int Level1Scene::m_spawnObject(PathFindingDisplayObject* object)
{
	//m_resetGrid();

	Tile* randomTile = nullptr;
	auto randomTileIndex = 0;
	do
	{
		randomTileIndex = int(Util::RandomRange(0, m_pGrid.size() - 1));
		randomTile = m_pGrid[randomTileIndex];
	} while (randomTile->getTileState() != UNVISITED); // search for empty tile


	if (object->getTile() != nullptr)
	{
		object->getTile()->setTileState(UNVISITED);
	}

	object->setPosition(randomTile->getPosition());
	object->setTile(randomTile);

	return randomTileIndex;
}

void Level1Scene::m_spawnObstacles()
{

	for (Obstacle* obstacle : m_pObstacleVec)
	{
		m_spawnObject(obstacle);
		obstacle->getTile()->setTileState(IMPASSABLE);
		for (int i = 0; i < 4; i++)
		{
			if(obstacle->getTile()->getNeighbours()[i] != nullptr)
			obstacle->getTile()->getNeighbours()[i]->setTileState(CLOSED);
		}
	}

}

void Level1Scene::m_spawnEnemy()
{
	for (Enemy* enemy : m_pEnemyVec)
	{
		m_spawnObject(enemy);
	}
}

void Level1Scene::m_checkCollisions()
{
	// first, reset all variables to their initial states
	for (unsigned int i = 0; i < m_pEnemyVec.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			m_pEnemyVec[i]->setFeeler(j, false);
			m_pEnemyVec[i]->setLOS(false);
			m_pEnemyVec[i]->setSmell(false);
		}
	}
	for (unsigned int i = 0; i < m_pTilesBehindCover.size(); i++)
	{
		m_pTilesBehindCover[i]->setBehindCover(false);
	}
	m_pTilesBehindCover.clear();
	m_pTilesBehindCover.shrink_to_fit();

	for (unsigned int i = 0; i < m_pObstacleVec.size(); i++)
	{
		// Handle player collisions
		if (CollisionManager::circleAABBCheck(m_pPlayer, m_pObstacleVec[i]))
		{
			TheSoundManager::Instance()->playSound("ObstacleBumped", 0);

			// TAKE 5!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			glm::vec2 pVel = m_pPlayer->getVelocity();
			glm::vec2 pPos = m_pPlayer->getPosition() - (pVel * 2.25f);
			int pW = m_pPlayer->getWidth();
			int pH = m_pPlayer->getHeight();
			glm::vec2 oPos = m_pObstacleVec[i]->getPosition();
			int oW = m_pObstacleVec[i]->getWidth();
			int oH = m_pObstacleVec[i]->getHeight();
			
			SDL_Rect pRect = { pPos.x - (pW / 2), pPos.y - (pW / 2), pW, pH };
			SDL_Rect oRect = { oPos.x - (oW / 2), oPos.y - (oW / 2), oW, oH };
			
			if (pRect.x + pRect.w <= oRect.x)
			{
				m_pPlayer->setVelocity({ 0, pVel.y });
				pPos.y += (pVel.y * 2.25f);
			}
			else if (pRect.x >= oRect.x + oRect.w)
			{
				m_pPlayer->setVelocity({ 0, pVel.y });
				pPos.y += (pVel.y * 2.25f);
			}
			else if (pRect.y + pRect.h <= oRect.y)
			{
				m_pPlayer->setVelocity({ pVel.x, 0 });
				pPos.x += (pVel.x * 2.25f);
			}
			else if (pRect.y >= oRect.y + oRect.h)
			{
				m_pPlayer->setVelocity({ pVel.x, 0 });
				pPos.x += (pVel.x * 2.25f);
			}

			m_pPlayer->setPosition(pPos);

			// TAKE 4!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//glm::vec2 pVel = m_pPlayer->getVelocity();
			//glm::vec2 pPos = m_pPlayer->getPosition() - pVel;
			//int pW = m_pPlayer->getWidth();
			//int pH = m_pPlayer->getHeight();
			//glm::vec2 oPos = m_pObstacleVec[i]->getPosition();
			//int oW = m_pObstacleVec[i]->getWidth();
			//int oH = m_pObstacleVec[i]->getHeight();
			//float pLef = pPos.x - (pW * 0.5f);
			//float pRig = pPos.x + (pW * 0.5f);
			//float pTop = pPos.y - (pH * 0.5f);
			//float pBot = pPos.y + (pH * 0.5f);
			//float oLef = oPos.x - (oW * 0.5f);
			//float oRig = oPos.x + (oW * 0.5f);
			//float oTop = oPos.y - (oH * 0.5f);
			//float oBot = oPos.y + (oH * 0.5f);
			//
			//if (pRig < oLef)
			//{
			//	pPos.x = oLef - (pW * 0.5f);
			//}
			//else if (pLef > oRig)
			//{
			//	pPos.x = oRig + (pW * 0.5f);
			//}
			//else if (pTop < oBot)
			//{
			//	pPos.y = oBot + (pH * 0.5f);
			//}
			//else if (pBot > oTop)
			//{
			//	pPos.y = oTop - (pH * 0.5f);
			//}
			//m_pPlayer->setPosition(pPos);

			// TAKE 3!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//float dist = CollisionManager::circleAABBsquaredDistance(m_pPlayer->getPosition(), m_pPlayer->getWidth(), m_pObstacleVec[i]->getPosition(), m_pObstacleVec[i]->getWidth(), m_pObstacleVec[i]->getHeight());
			//dist = sqrt(dist);
			//std::cout << "dist: " << dist << std::endl;
			//glm::vec2 relativePos = m_pObstacleVec[i]->getPosition() - m_pPlayer->getPosition();
			//float pVelMag = Util::magnitude(m_pPlayer->getVelocity());
			//float rejectMag = pVelMag == 0.0f ? 0.0f : dist / pVelMag;
			//glm::vec2 reject = Util::normalize(relativePos) * rejectMag;
			//m_pPlayer->setPosition(m_pPlayer->getPosition() - reject);
			//m_pPlayer->setVelocity({ 0, 0 });

			// TAKE 2!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//glm::vec2 pVel = m_pPlayer->getVelocity();
			//glm::vec2 pPos = m_pPlayer->getPosition() - pVel;
			//int pW = m_pPlayer->getWidth();
			//int pH = m_pPlayer->getHeight();
			//glm::vec2 oPos = m_pObstacleVec[i]->getPosition();
			//int oW = m_pObstacleVec[i]->getWidth();
			//int oH = m_pObstacleVec[i]->getHeight();
			//
			//SDL_Rect pRect = { pPos.x - (pW / 2), pPos.y - (pW / 2), pW, pH };
			//SDL_Rect oRect = { oPos.x - (oW / 2), oPos.y - (oW / 2), oW, oH };
			//
			//if (pRect.x + pRect.w > oRect.x)
			//{
			//	pPos.x = pPos.x - pVel.x;
			//	//pPos.x = pPos.x - ((pRect.x + pRect.w) - oRect.x);
			//	//std::cout << ((pRect.x + pRect.w) - oRect.x);
			//}
			//if (pRect.x < oRect.x + oRect.w)
			//{
			//	pPos.x = pPos.x - pVel.x;
			//	//pPos.x = pPos.x - (pRect.x - (oRect.x + oRect.w));
			//}
			//if (pRect.y + pRect.h > oRect.y)
			//{
			//	pPos.y = pPos.y - pVel.y;
			//	//pPos.y = pPos.y - ((pRect.y + pRect.h) - oRect.y);
			//}
			//if (pRect.y < oRect.y + oRect.h)
			//{
			//	pPos.y = pPos.y - pVel.y;
			//	//pPos.y = pPos.y - (pRect.y - (oRect.y + oRect.h));
			//}
			//m_pPlayer->setPosition(pPos);
			//m_pPlayer->setVelocity({ 0, 0 });
			
			// TAKE 1!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//m_pPlayer->setVelocity(m_pPlayer->getVelocity() * glm::vec2{ -0.8 ,-0.8 });
			//m_pPlayer->move();
		}

		// Handle enemy collisions
		for (unsigned int j = 0; j < m_pEnemyVec.size(); j++)
		{
			// Check if colliding with obstacles
			if (CollisionManager::circleAABBCheck(m_pEnemyVec[j], m_pObstacleVec[i]))
			{
				m_pEnemyVec[j]->setVelocity(m_pEnemyVec[j]->getVelocity() * glm::vec2{ -1.0, -1.0 });
				m_pEnemyVec[j]->move();
				m_pEnemyVec[j]->setVelocity(glm::vec2{ 0, 0 });
			}
					// Check feelers
					for (unsigned int feeler = 0; feeler < 3; feeler++)
					{
						if (CollisionManager::lineAABBCheck(m_pEnemyVec[j]->getPosition(), m_pEnemyVec[j]->getFeelerEndPosition(feeler), m_pObstacleVec[i]))
						{
							m_pEnemyVec[j]->setFeeler(feeler, true);
						}
					}

					// Check LOS
					{
						glm::vec2 enemyPos = m_pEnemyVec[j]->getPosition();
						glm::vec2 playerPos = m_pPlayer->getPosition();
						glm::vec2 dirEtoP = playerPos - enemyPos;
						glm::vec2 dirHofE = m_pEnemyVec[j]->getDirection();
						float angle = Util::angle(dirHofE, dirEtoP);
						float FOV = m_pEnemyVec[j]->getFOV();

						if (angle < FOV && angle > -FOV)
						{ // Check the angle between the enemy's heading and the player - the enemy cannot see the player if the player is behind it
							m_pEnemyVec[j]->setLOS(true);
							//std::cout << "You're in my field of vision" << std::endl;
							if (CollisionManager::lineAABBCheck(enemyPos, playerPos, m_pObstacleVec[i]))
							{ // NOW check whether there's an obstacle in the way
								//std::cout << " I see you!" << std::endl; // this is backwards
								m_pEnemyVec[j]->setLOS(false);
							}
						}
					}


					// Check Smell
					if (CollisionManager::squaredRadiusCheck(m_pEnemyVec[j]->getPosition(), m_pEnemyVec[j]->getSmellRadius(), m_pPlayer))
					{
						m_pEnemyVec[j]->setSmell(true);
						//	m_pEnemyVec[j]->setTargetPosition(m_pPlayer->getPosition());
					}
				}
		for (unsigned int enemy = 0; enemy < m_pEnemyVec.size(); enemy++)
		{
			if (CollisionManager::circleAABBCheck(m_pEnemyVec[enemy]->getCollider(), m_pPlayer))
			{
				if (m_pPlayer->changeHealth(m_pEnemyVec[enemy]->getDamage()))
				{
					m_iCurrentPts = 0;
					m_ScreenMsg->setText("You died! Score reset to 0.");
					m_iScreenMsgFrame = 0;
					m_ScreenMsg->setAlpha(255);
				}
			}
			if (CollisionManager::circleAABBCheck(m_pEnemyVec[enemy], m_pPlayer->getCollider()))
			{
				m_pEnemyVec[enemy]->setVelocity(m_pEnemyVec[enemy]->getVelocity() * -20.2f);
				m_pEnemyVec[enemy]->setPosition(m_pEnemyVec[enemy]->getPosition() + m_pEnemyVec[enemy]->getVelocity());
				TheSoundManager::Instance()->playSound("EnemyMeleed", 0);




				if (m_pEnemyVec[enemy]->changeHealth(-m_pPlayer->m_iMeleeDamage))
				{
					TheSoundManager::Instance()->playSound("EnemyDie", 0);
					m_iRespawnFrame = 0;
					int ptsVal = m_pEnemyVec[enemy]->getPtsValue();
					m_iCurrentPts += ptsVal;
					
					if (m_iCurrentPts < m_iTotalPts)
					{
						m_ScreenMsg->setText("Enemy killed +" + std::to_string(ptsVal) + " points!");
					}
					else
					{
						m_iTotalPts = static_cast<int>(m_iTotalPts * 2);
						m_iCurrentPts = 0;
						m_ScreenMsg->setText("Congratulations! Target score reached! New target: " + std::to_string(m_iTotalPts) + '!');
					}
					m_iScreenMsgFrame = 0;
					m_ScreenMsg->setAlpha(255);

					removeChildByIndex(m_pEnemyVec[enemy]->DisplayListIndexInScene);
					delete m_pEnemyVec[enemy];
					m_pEnemyVec[enemy] = nullptr;
					m_pEnemyVec.erase(m_pEnemyVec.begin() + enemy);
				}
			}
		}

				// Handle bullet collisions
				for (unsigned int j = 0; j < m_pPlayer->getBullets().size(); j++)
				{
					// Bullet collision with obstacles
					Bullet* bullet = m_pPlayer->getBullets()[j];
					if (CollisionManager::AABBCheck(m_pObstacleVec[i], m_pPlayer->getBullets()[j]))
					{
						TheSoundManager::Instance()->playSound("ObstacleShot", 0);
						delete bullet;
						m_pPlayer->getBullets()[j] = nullptr;
						m_pPlayer->getBullets().erase(m_pPlayer->getBullets().begin() + 1 * j);
						break;
					}
					// Bullet collision with enemies
					else
					{
						for (unsigned int enemy = 0; enemy < m_pEnemyVec.size(); enemy++)
						{
							if (CollisionManager::circleAABBCheck(m_pEnemyVec[enemy], m_pPlayer->getBullets()[j]))
							{
								TheSoundManager::Instance()->playSound("EnemyHit", 0);
								// deal damage to the enemy
								if (m_pEnemyVec[enemy]->changeHealth(-m_pPlayer->getBullets()[j]->getDamage()))
								{
									TheSoundManager::Instance()->playSound("EnemyDie", 0);
									m_iRespawnFrame = 0;
									int ptsVal = m_pEnemyVec[enemy]->getPtsValue();
									m_iCurrentPts += ptsVal;

									if (m_iCurrentPts < m_iTotalPts)
									{
										m_ScreenMsg->setText("Enemy killed +" + std::to_string(ptsVal) + " points!");
									}
									else
									{
										m_iTotalPts = static_cast<int>(m_iTotalPts * 2);
										m_iCurrentPts = 0;
										m_ScreenMsg->setText("Congratulations! Target score reached! New target: " + std::to_string(m_iTotalPts) + '!');
									}
									m_iScreenMsgFrame = 0;
									m_ScreenMsg->setAlpha(255);

									removeChildByIndex(m_pEnemyVec[enemy]->DisplayListIndexInScene);
									delete m_pEnemyVec[enemy];
									m_pEnemyVec[enemy] = nullptr;
									m_pEnemyVec.erase(m_pEnemyVec.begin() + enemy);
								}

								// delete the bullet
								delete bullet;
								m_pPlayer->getBullets()[j] = nullptr;
								m_pPlayer->getBullets().erase(m_pPlayer->getBullets().begin() + 1 * j);
							}
						}
					}
				}
				m_pPlayer->getBullets().shrink_to_fit();

				// Populate the tiles behind cover vector
				for (unsigned int gridTile = 0; gridTile < m_pGrid.size(); gridTile++)
				{
					if (m_pGrid[gridTile]->getTileState() == CLOSED)
					{
						if (CollisionManager::lineAABBCheck(m_pGrid[gridTile]->getPosition(), m_pPlayer->getPosition(), m_pObstacleVec[i]))
						{
							m_pGrid[gridTile]->setBehindCover(true);
							m_pTilesBehindCover.push_back(m_pGrid[gridTile]);
						}
					}
				}
			}
		}

std::vector<Tile*>& Level1Scene::getTilesBehindCover()
{
	return m_pTilesBehindCover;
}

glm::vec2 Level1Scene::getNearestCoverPoint(const glm::vec2 position)
{
	if (m_pTilesBehindCover.empty())
	{
		std::cout << "There were no tiles behind cover!" << std::endl;
		return position;
	}
	
	glm::vec2 nearestCoverPoint = m_pTilesBehindCover[0]->getPosition();
	float sqrmagFromCover = Util::squaredMagnitude(nearestCoverPoint - position);

	for (unsigned int i = 1; i < m_pTilesBehindCover.size(); i++)
	{
		glm::vec2 currentTilePosition = m_pTilesBehindCover[i]->getPosition();
		float sqrmagFromCurrent = Util::squaredMagnitude(currentTilePosition - position);
		if (sqrmagFromCurrent < sqrmagFromCover)
		{
			nearestCoverPoint = currentTilePosition;
			sqrmagFromCover = sqrmagFromCurrent;
		}
	}

	return nearestCoverPoint;
}

glm::vec2 Level1Scene::getFurthestCoverPoint(const glm::vec2 position)
{
	if (m_pTilesBehindCover.empty())
	{
		std::cout << "There were no tiles behind cover!" << std::endl;
		return position;
	}

	glm::vec2 nearestCoverPoint = m_pTilesBehindCover[0]->getPosition();
	float sqrmagFromCover = Util::squaredMagnitude(nearestCoverPoint - position);

	for (unsigned int i = 1; i < m_pTilesBehindCover.size(); i++)
	{
		glm::vec2 currentTilePosition = m_pTilesBehindCover[i]->getPosition();
		float sqrmagFromCurrent = Util::squaredMagnitude(currentTilePosition - position);
		if (sqrmagFromCurrent > sqrmagFromCover)
		{
			nearestCoverPoint = currentTilePosition;
			sqrmagFromCover = sqrmagFromCurrent;
		}
	}

	return nearestCoverPoint;
}

glm::vec2 Level1Scene::getNearestPatrolPoint(const glm::vec2 position)
{
	if (m_pPatrolPoints.empty())
	{
		std::cout << "There were no tiles behind cover!" << std::endl;
		return position;
	}

	glm::vec2 nearestPatrolPoint = m_pPatrolPoints[0]->getPosition();
	float sqrmagFromCover = Util::squaredMagnitude(nearestPatrolPoint - position);

	for (unsigned int i = 1; i < m_pPatrolPoints.size(); i++)
	{
		glm::vec2 currentTilePosition = m_pPatrolPoints[i]->getPosition();
		float sqrmagFromCurrent = Util::squaredMagnitude(currentTilePosition - position);
		if (sqrmagFromCurrent > sqrmagFromCover)
		{
			nearestPatrolPoint = currentTilePosition;
			sqrmagFromCover = sqrmagFromCurrent;
		}
	}

	return nearestPatrolPoint;
}

glm::vec2 Level1Scene::getRandomPatrolPoint(int number)
{
	
	return m_pPatrolPoints[number]->getPosition();
}

void Level1Scene::updateEnemyTargets()
{
	for (unsigned int j = 0; j < m_pEnemyVec.size(); j++)
	{
		m_pEnemyVec[j]->m_playersPos = m_pPlayer->getPosition();
		m_pEnemyVec[j]->m_nearestCoverTile = getNearestCoverPoint(m_pEnemyVec[j]->getPosition());
		m_pEnemyVec[j]->m_furthestCoverTile = getFurthestCoverPoint(m_pPlayer->getPosition());
		m_pEnemyVec[j]->m_patrolPoint = getRandomPatrolPoint(m_pEnemyVec[j]->getMySpecialNumber());

	}
}
