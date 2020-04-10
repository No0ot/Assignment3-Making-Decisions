#pragma once
#ifndef __LEVEL1_SCENE__
#define __LEVEL1_SCENE__

#include "Scene.h"


#include "PlaneSprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "Enemy.h"

class Level1Scene : public Scene
{
public:
	Level1Scene();
	~Level1Scene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void m_buildGrid();
	void m_mapTiles();
	int m_spawnObject(PathFindingDisplayObject*);
	void m_spawnObstacles();
	void m_spawnEnemy();
private:
	glm::vec2 m_mousePosition;
	std::vector<Tile*> m_pGrid;
	PlaneSprite* m_pPlaneSprite;
	Player* m_pPlayer;
	std::vector<Obstacle*> m_pObstacleVec;
	std::vector<Enemy*> m_pEnemyVec;

};

#endif /* defined (__LEVEL1_SCENE__) */