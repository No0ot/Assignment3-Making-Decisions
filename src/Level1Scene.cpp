#include "Level1Scene.h"
#include "Game.h"
#include "ExplosionManager.h"
#include "Util.h"

Level1Scene::Level1Scene()
{
	Level1Scene::start();
}

Level1Scene::~Level1Scene()
= default;

void Level1Scene::draw()
{
	drawDisplayList();
	ExplosionManager::Instance()->draw();
}

void Level1Scene::update()
{
	updateDisplayList();
	ExplosionManager::Instance()->update();
	m_pPlayer->update();

	for (int i = 0; i < (int)m_pObstacleVec.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pPlayer,m_pObstacleVec[i] ))
		{
			m_pPlayer->setVelocity(m_pPlayer->getVelocity() * glm::vec2{ -0.8 ,-0.8 });
		}
		for (int j = 0; j < (int)m_pPlayer->getBullets().size(); j++)
		{

			Bullet* bullet = m_pPlayer->getBullets()[j];
			if (CollisionManager::AABBCheck(m_pObstacleVec[i], m_pPlayer->getBullets()[j] ))
			{
				delete bullet;
				m_pPlayer->getBullets()[j] = nullptr;
				m_pPlayer->getBullets().erase(m_pPlayer->getBullets().begin() + 1 * j);
				break;
			}
		}
				m_pPlayer->getBullets().shrink_to_fit();
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
			auto explosion = ExplosionManager::Instance()->getExplosion();
			explosion->setPosition(m_mousePosition);
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
					m_pPlayer->setAnimationState(PLAYER_MELEE);

				}

				if (keyPressed == SDLK_SPACE)
				{
					m_pPlayer->setAnimationState(PLAYER_SHOOT);
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

	m_pPlaneSprite = new PlaneSprite();
	addChild(m_pPlaneSprite);

	m_pPlayer = new Player();
	addChild(m_pPlayer);


	for (int i = 0; i < 20; i++)
	{
		m_pObstacleVec.push_back(new Obstacle());
		addChild(m_pObstacleVec.back());

	}
	m_spawnObstacles();
	for (int i = 0; i < 2; i++)
	{
		m_pEnemyVec.push_back(new Enemy());
		addChild(m_pEnemyVec.back());
	}

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