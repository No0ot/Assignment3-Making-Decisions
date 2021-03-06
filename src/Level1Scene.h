#pragma once
#ifndef __LEVEL1_SCENE__
#define __LEVEL1_SCENE__

#include "Scene.h"


#include "PlaneSprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "Melee_Enemy.h"
#include "Ranged_Enemy.h"
#include "PatrolPoint.h"

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
	void m_checkCollisions();
	std::vector<Tile*>& getTilesBehindCover();
	glm::vec2 getNearestPatrolPoint(const glm::vec2 position);
	glm::vec2 getRandomPatrolPoint(int number);
	glm::vec2 getNearestCoverPoint(const glm::vec2 position);
	glm::vec2 getFurthestCoverPoint(const glm::vec2 position);
	void updateEnemyTargets();
private:
	glm::vec2 RandomPatrolPoint;
	glm::vec2 m_mousePosition;
	std::vector<Tile*> m_pGrid;
	PlaneSprite* m_pPlaneSprite;
	Player* m_pPlayer;
	std::vector<Obstacle*> m_pObstacleVec;
	std::vector<Tile*> m_pTilesBehindCover;
	std::vector<Enemy*> m_pEnemyVec;
	std::vector<PatrolPoint*> m_pPatrolPoints;

	int m_iTotalPts;
	int m_iCurrentPts;
	UIBar m_PtsBar;

	int m_iRespawnFrame;
	int m_iRespawnFrameMax;
	UIBar m_RespawnBar;
	
	int m_iScreenMsgFrame;
	int m_iScreenMsgFrameMax;
	Label* m_ScreenMsg{};
	
	Melee_Enemy::Loadout m_MeleeLoadouts[4];
	Ranged_Enemy::Loadout m_RangedLoadouts[4];
};

#endif /* defined (__LEVEL1_SCENE__) */