#pragma once
#include "player.h"
#include "enemy.h"
#include "boostUps.h"

class Game {
	
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	BoostUps* boost = nullptr;
	std::vector<Enemy*> enemies;
	std::vector< Bullet*> restBullets;
	graphics::Brush br,UIbr;
	void spawnEnemy();
	void checkEnemy();
	void spawnBoostUps();
	void checkBullets();
	bool checkCollision(Enemy*);
	bool checkBullet(Bullet*, Enemy*);
	bool checkEnemyBullet(Bullet*, Player*);
	bool checkBoost(BoostUps*);
	bool checkRestBullet(Bullet*, Player*);
	int maxEnemies;
	int score;
	float enemyTime;
	float enemyPosition;
	float boostTimer;
	float glow;
	float gameLife;
	int Status;
	int levels;
public:
	void updateStartingScreen();
	void updateGame();
	void updateGameOver();
	void drawStartingScreen();
	void drawGame();
	void drawGameOver();
	void draw();
	void update();
	void loseLife(Enemy*);
	void init();
	void clearGame();
	Game();
	~Game();
};