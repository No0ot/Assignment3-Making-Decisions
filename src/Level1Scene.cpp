#include "Level1Scene.h"
#include "Game.h"
#include "ExplosionManager.h"

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
	m_pPlaneSprite = new PlaneSprite();
	addChild(m_pPlaneSprite);

	m_pPlayer = new Player();
	addChild(m_pPlayer);
}
