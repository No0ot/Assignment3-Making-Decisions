#include "Level1Scene.h"
#include "Game.h"
#include "ExplosionManager.h"
#include "Util.h"

Level1Scene::Level1Scene() : m_iCurrentPts(0), m_iTotalPts(100), m_PtsBar(10, 10, m_iCurrentPts, m_iTotalPts, 2.0f, { 192, 192, 255, 192 })
{
	Level1Scene::start();
	TheSoundManager::Instance()->load("../Assets/audio/EnemyHit.wav", "EnemyHit", sound_type::SOUND_SFX);
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
	m_PtsBar.draw();
}

void Level1Scene::update()
{
	updateDisplayList();
	m_pPlayer->update();
	updateEnemyTargets();
	m_checkCollisions();
	/*for (unsigned int enemy = 0; enemy < m_pEnemyVec.size(); enemy++)
	{
		if (m_pEnemyVec[enemy]->canDetect())
		{
			m_pEnemyVec[enemy]->setTargetPosition(m_pPlayer->getPosition());
		}
		
		glm::vec2 coverPosition = getNearestCoverPoint(m_pEnemyVec[enemy]->getPosition());
		m_pEnemyVec[enemy]->setTargetPosition(coverPosition);*/
//	}
	m_PtsBar.update();
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
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
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
				if(keyPressed == SDLK_w)
				{
					std::cout << "move forward" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_RUN);
					TheSoundManager::Instance()->playSound("PlayerStep", 0);
					m_pPlayer->moveForward();;
				}

				if (keyPressed == SDLK_a)
				{
					//std::cout << "move left" << std::endl;
				    m_pPlayer->setAnimationState(PLAYER_IDLE);
					m_pPlayer->turnLeft();
				}

				if (keyPressed == SDLK_s)
				{
					std::cout << "move back" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_RUN);
					m_pPlayer->moveBack();
				}

				if (keyPressed == SDLK_d)
				{
					//std::cout << "move right" << std::endl;
					m_pPlayer->setAnimationState(PLAYER_IDLE);
					m_pPlayer->turnRight();
				}

				if (keyPressed == SDLK_f)
				{
					TheSoundManager::Instance()->playSound("PlayerMelee", 0);
					m_pPlayer->setAnimationState(PLAYER_MELEE);

				}

				if (keyPressed == SDLK_SPACE)
				{
					m_pPlayer->setAnimationState(PLAYER_SHOOT);
					TheSoundManager::Instance()->playSound("PlayerShoot", 0);
					m_pPlayer->shoot();

				}

				if (keyPressed == SDLK_c)
				{
					m_pPlayer->turnaround();

				}
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
	m_buildGrid();
	m_mapTiles();
	for (int i = 0; i < 8; i++)
	{
		m_pObstacleVec.push_back(new Obstacle());
		addChild(m_pObstacleVec.back());

	}
	m_spawnObstacles();
	for (int i = 0; i < 1; i++)
	{
		m_pEnemyVec.push_back(new Melee_Enemy());
		addChild(m_pEnemyVec.back());
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
			m_pEnemyVec[i]->setLOS(true);
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
			m_pPlayer->setVelocity(m_pPlayer->getVelocity() * glm::vec2{ -0.8 ,-0.8 });
			m_pPlayer->move();
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
			if (CollisionManager::lineAABBCheck(m_pEnemyVec[j]->getPosition(), m_pPlayer->getPosition(), m_pObstacleVec[i]))
			{
				m_pEnemyVec[j]->setLOS(false);
			
				//m_pEnemyVec[j]->setTargetPosition(m_pPlayer->getPosition());
			}
	

			// Check Smell
			if (CollisionManager::squaredRadiusCheck(m_pEnemyVec[j]->getPosition(), m_pEnemyVec[j]->getSmellRadius(), m_pPlayer))
			{
				m_pEnemyVec[j]->setSmell(true);
			//	m_pEnemyVec[j]->setTargetPosition(m_pPlayer->getPosition());
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
		if (sqrmagFromCurrent < sqrmagFromCover)
		{
			nearestPatrolPoint = currentTilePosition;
			sqrmagFromCover = sqrmagFromCurrent;
		}
	}

	return nearestPatrolPoint;
}

glm::vec2 Level1Scene::getRandomPatrolPoint()
{
	int temp = rand() % 4;
	RandomPatrolPoint = m_pPatrolPoints[temp]->getPosition();
	return RandomPatrolPoint;
}

void Level1Scene::updateEnemyTargets()
{
	for (unsigned int j = 0; j < m_pEnemyVec.size(); j++)
	{
		
		switch (m_pEnemyVec[j]->getBehaviour())
		{
		case BehaviourState::PATROL:
			m_pEnemyVec[j]->setTargetPosition(getNearestPatrolPoint(m_pEnemyVec[j]->getPosition()));
			break;
		case BehaviourState::PATROL2:
			m_pEnemyVec[j]->setTargetPosition(m_pPatrolPoints[m_pEnemyVec[j]->randomnum]->getPosition());
			break;
		case BehaviourState::ASSAULT:
			m_pEnemyVec[j]->setTargetPosition(m_pPlayer->getPosition());
			break;
		case BehaviourState::FLEE:
			m_pEnemyVec[j]->setTargetPosition(m_pPlayer->getPosition());
			break;
		case BehaviourState::COWER:
			m_pEnemyVec[j]->setTargetPosition(getNearestCoverPoint(m_pEnemyVec[j]->getPosition()));
			break;
		}

	}
}
