#include "game.h"
#include "graphics.h"
#include "config.h"
#include "util.h"

void Game::draw() {

	if (Status == 0) drawStartingScreen();
	else if (Status == 1) drawGame();
	else drawGameOver();
}
void Game::drawStartingScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "intro.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	char info[30];
	sprintf_s(info, "SPACE WARS");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2 - 100, 100, info, br);
	sprintf_s(info, "PRESS -ENTER- TO PLAY");
	graphics::drawText(CANVAS_WIDTH / 2 - 300, CANVAS_HEIGHT / 2 + 20, 40, info, br);
	sprintf_s(info, "PRESS -ESC- TO QUIT");
	graphics::drawText(CANVAS_WIDTH / 2 - 300, CANVAS_HEIGHT / 2 + 80, 40, info, br);

}
void Game::drawGame()
{	//BACKGROUND
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "universe.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	//DRAW PLAYER
	if (player) {
		player->draw();
	}
	//DRAW ENEMIES
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isActive()) {
			enemies[i]->draw();
		}
	}
	//DRAW BULLETS FROM DEAD ENEMIES
	for (int i = 0; i < restBullets.size(); i++) {
		restBullets[i]->draw();
	}
	//DRAW BOOST
	if (boost) {
		boost->draw();
	}
	//DRAW PLAYER LIFE / HEARTS
	br.texture = " ";
	br.texture = std::string(ASSET_PATH) + "heart.png";
	br.outline_opacity = 0.f;
	if (player) {
		if (player->getLife() == 3) {
			graphics::drawRect(25, CANVAS_HEIGHT - 25, 50, 50, br);
			graphics::drawRect(75, CANVAS_HEIGHT - 25, 50, 50, br);
			graphics::drawRect(125, CANVAS_HEIGHT - 25, 50, 50, br);
		}
		else if (player->getLife() == 2) {
			graphics::drawRect(25, CANVAS_HEIGHT - 25, 50, 50, br);
			graphics::drawRect(75, CANVAS_HEIGHT - 25, 50, 50, br);
		}
		else if (player->getLife() == 1) {
			graphics::drawRect(25, CANVAS_HEIGHT - 25, 50, 50, br);
		}
	}


	//User Interface
	char info[10];
	sprintf_s(info, "SCORE");
	graphics::drawText(CANVAS_WIDTH - 300, CANVAS_HEIGHT - 10, 40, info, br);
	sprintf_s(info, "(%i)", score);
	graphics::drawText(CANVAS_WIDTH - 110, CANVAS_HEIGHT - 10, 40, info, br);
	sprintf_s(info, "LIFE");
	graphics::drawText(200, CANVAS_HEIGHT - 10, 40, info, br);
	//DRAW GAME LIFE
	UIbr.outline_opacity = 0.0f;
	UIbr.fill_color[0] = 1.0f;
	UIbr.fill_color[1] = 0.0f;
	UIbr.fill_color[2] = 0.0f;
	UIbr.texture = "";
	UIbr.fill_secondary_color[0] = (1.0f - gameLife) * 1.0f;
	UIbr.fill_secondary_color[1] = 0.0f;
	UIbr.fill_secondary_color[2] = 1.0f * gameLife;
	UIbr.gradient = true;
	UIbr.gradient_dir_u = 1.0f;
	UIbr.gradient_dir_v = 0.0f;
	graphics::drawRect(450 - (1.0f - gameLife) * 200 / 2, CANVAS_HEIGHT - 22, 200 * gameLife, 20, UIbr);


}//DRAW
void Game::drawGameOver()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "intro.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	char info[50];
	sprintf_s(info, "GAME OVER");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2 - 100, 40, info, br);
	sprintf_s(info, "SCORE");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2 - 50, 40, info, br);
	sprintf_s(info, "(%i)", score);
	graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 50, 40, info, br);
	sprintf_s(info, "PRESS -ENTER- TO Restart");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2, 40, info, br);
	sprintf_s(info, "PRESS -ESC- TO QUIT");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT / 2 + 50, 40, info, br);

}
void Game::update() {
	if (Status == 0) updateStartingScreen();
	else if (Status == 1) updateGame();
	else updateGameOver();

}
void Game::updateStartingScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
		Status = 1;
		player = new Player();
	}
}
void Game::updateGame()
{

	//UPDATE PLAYER
	if (player) 
		player->update();
	//SPAWN ENEMIES
	spawnEnemy();
	//SPAWN BOOST OR DELETE IF OUT OF BOUNDS
	if (!boost) {
		spawnBoostUps();
	}
	else {
		if (!boost->isAlive()) {
			delete boost;
			boost = nullptr;
		}
	}
	//UPDATE BOOST
	if (boost) {
		boost->update();
	}
	//UPDATE ENEMIES
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isActive()) {
			enemies[i]->update();
		}
	}//UPDATE BULLETS FROM DEAD ENEMIES
	for (int i = 0; i < restBullets.size(); i++) {
		restBullets[i]->update();
	}
	//CHECK PLAYER COLLISION WITH ENEMIES
	for (int i = 0; i < enemies.size(); i++) {
		if (checkCollision(enemies[i]) && enemies[i]->isAlive()) {
			enemies[i]->setnotAlive();
			graphics::playSound(std::string(ASSET_PATH) + "enemyExplosion.mp3", 0.2f, false);
			if (!player->isShielded()) {
				player->setLife(true);
				player->setShield();
			}
			else score += 1;
		}
		if (player->getLife() < 1) {
			Status = 2;
			graphics::playSound(std::string(ASSET_PATH) + "lost.mp3", 0.3f, false);
			return;
		}
	}
	//CHECK PLAYER BULLETS COLLISION WITH ENEMIES
	for (int i = 0; i < player->bullets.size(); i++) {
		for (int j = 0; j < enemies.size(); j++)
			if (checkBullet(player->bullets[i], enemies[j]) && enemies[j]->isAlive()) {
				enemies[j]->setnotAlive();
				graphics::playSound(std::string(ASSET_PATH) + "enemyExplosion.mp3", 0.2f, false);
				player->bullets[i]->setnotAlive();
				score += 1;
			}
	}
	//CHECK ENEMIES BULLETS COLLISION WITH PLAYER
	for (int i = 0; i < enemies.size(); i++) {
		for (int j = 0; j < enemies[i]->bullets.size(); j++)
			if (checkEnemyBullet(enemies[i]->bullets[j], player)) {
				enemies[i]->bullets[j]->setnotAlive();
				player->setLife(true);
				graphics::playSound(std::string(ASSET_PATH) + "explosion.mp3", 0.3f, false);
				if (!player->isShielded()) {
					player->setShield();
				}
				if (player->getLife() < 1) {
					Status = 2;
					graphics::playSound(std::string(ASSET_PATH) + "lost.mp3", 0.2f, false);
					return;
				}
			}
	}
	//CHECK DEAD ENEMIES BULLETS COLLISION WITH PLAYER
	for (int i = 0; i < restBullets.size(); i++) {
		if (checkEnemyBullet(restBullets[i], player)) {
			restBullets[i]->setnotAlive();
			player->setLife(true);
			graphics::playSound(std::string(ASSET_PATH) + "explosion.mp3", 0.3f, false);
			if (!player->isShielded()) {
				player->setShield();
			}
			if (player->getLife() < 1) {
				Status = 2;
				graphics::playSound(std::string(ASSET_PATH) + "lost.mp3", 0.2f, false);
				return;
			}
		}
	}
	//CHECK PLAYER COLLISION WITH BOOST
	if (checkBoost(boost)) {
		graphics::playSound(std::string(ASSET_PATH) + "boost.mp3", 0.3f, false);
		if (boost->getType() == 0) {
			player->setShield();
			player->setLife(false);

		}
		else if (boost->getType() == 1) {
			player->setnoReload();
		}
		else {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->setTimeSlow();
			}
			for (int i = 0; i < restBullets.size(); i++) {
				restBullets[i]->setTimeSlow(graphics::getGlobalTime() + 3000.0f);
			}
		}
		delete boost;
		boost = nullptr;
	}
	//CHECK ALL BULLETS
	checkBullets();
	//LOSE LIFE
	for (int i = 0;i < enemies.size(); i++)
	{
		loseLife(enemies[i]);
	}
	//CHECK ENEMIEs
	checkEnemy();
}
void Game::updateGameOver()
{
	clearGame();
	graphics::stopMusic();
	if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
		Status = 0;
		init();
	}
}
void Game::spawnEnemy()
{
	//SPAWN MORE ENEMIES AFTER 10 POINTS
	if (score > levels) {
		maxEnemies++;
		levels += 10;
	}
	//SPAWN ENEMY IF LESS THAN AN INT AND CERTAIN TIME HAS PASSED
	if (enemies.size()<maxEnemies && graphics::getGlobalTime() > enemyTime) {
			//ENEMY WITH THE POSITION OF LAST ENEMY SO THEY DON'T OVERLAP
			enemy = new Enemy(enemyPosition);
			enemies.push_back(enemy);
			enemyTime = graphics::getGlobalTime() + 400.0f;
			enemyPosition = enemy->getY();
	}
}
void Game::checkEnemy()
{
	for (int i = 0; i < enemies.size(); i++) {
		//IF ENEMY HIT GET POINTERS TO ITS BULLETS TO HAVE THEM CONTINUE 
		if (!enemies[i]->isAlive()) {
			for (int j = 0; j < enemies[i]->bullets.size(); j++) {
				restBullets.push_back(enemies[i]->bullets[j]);
				enemies[i]->bullets.clear();
			}
		}
		//IF ENEMY NOT ACTIVE DELETE ENEMY AND POINTER
		if (!enemies[i]->isActive()) {
			delete* (enemies.begin() + i);
			enemies.erase(enemies.begin() + i);
		}
	}
}
void Game::spawnBoostUps()
{	//IF BOOST DOESN'T EXIST AND AFTER SOME TIME SPAWN BOOST
	if (!boost && boostTimer < graphics::getGlobalTime()) {
		boost = new BoostUps(int(graphics::getGlobalTime()) % 3);
		boostTimer = graphics::getGlobalTime() + 10000.0f;
	}
}
void Game::checkBullets()
{	//DELETE PLAYER BULLETS AND POINTERS IF OUT OF BOUNDS OR HIT
	for (int i = 0; i < player->bullets.size(); i++) {
		if (!player->bullets[i]->isAlive()) {
			delete* (player->bullets.begin() + i);
			player->bullets.erase(player->bullets.begin() + i);
		}
	}
	//DELETE ENEMIES BULLETS AND POINTERS IF OUT OF BOUNDS OR HIT
	for (int i = 0; i < enemies.size(); i++) {
		for (int j = 0; j < enemies[i]->bullets.size(); j++) {
			if (!enemies[i]->bullets[j]->isAlive()) {
				delete* (enemies[i]->bullets.begin() + j);
				enemies[i]->bullets.erase(enemies[i]->bullets.begin() + j);
			}
		}
	}
	//DELETE DEAD ENEMY BULLETS AND POINTERS IF OUT OF BOUNDS OR HIT
	for (int i = 0; i <restBullets.size(); i++) {
		if (!restBullets[i]->isAlive()) {
			delete* (restBullets.begin() + i);
			restBullets.erase(restBullets.begin() + i);
		}
	}
}
bool Game::checkCollision(Enemy * enemy)
{	
	if (!player || !enemy) {
		return false;
	}
	Disk d1 = player->getCollisionHull();
	Disk d2 = enemy->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if(sqrt(dx*dx + dy*dy)< d1.radius + d2.radius)
		return true;
	else
		return false;
}
bool Game::checkRestBullet(Bullet* bullet, Player* player)
{
	if (!player || !enemy) {
		return false;
	}
	Disk d1 = bullet->getCollisionHull();
	Disk d2 = player->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		return true;
	else
		return false;
}
bool Game::checkBullet(Bullet* bullet, Enemy* enemy)
{
	if (!player || !enemy) {
		return false;
	}
	Disk d1 = bullet->getCollisionHull();
	Disk d2 = enemy->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		return true;
	else
		return false;
}
bool Game::checkEnemyBullet(Bullet* bullet, Player* player)
{
	if (!player || !enemy) {
		return false;
	}
	Disk d1 = bullet->getCollisionHull();
	Disk d2 = player->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		return true;
	else
		return false;
}
bool Game::checkBoost(BoostUps*)
{
	if (!player || !boost) {
		return false;
	}
	Disk d1 = boost->getCollisionHull();
	Disk d2 = player->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		return true;
	else
		return false;
}
void Game::clearGame()
{	//CLEAR ALL GAME DATA
	if (player) {
		for (int i = 0; i < player->bullets.size(); i++) {
			delete* (player->bullets.begin() + i);
		}
		player->bullets.clear();
		delete player;
		player = nullptr;
	}
	for (int i = 0; i < enemies.size(); i++)
		for (int j = 0; j < enemies[i]->bullets.size(); j++) {
			{	
				delete* (enemies[i]->bullets.begin() + j);
			}
			enemies[i]->bullets.clear();
			delete* (enemies.begin() + i);
		}
	enemies.clear();
	for (int i = 0; i < restBullets.size(); i++) {
		delete* (restBullets.begin() + i);
	}
	restBullets.clear();
	if (boost) {
		delete boost;
		boost = nullptr;
	}
}
void Game::loseLife(Enemy* enemy)
{	//LOSE LIFE IF YOU LET ENEMIES PASS
	if (enemy->getX() < 0) {
		gameLife -= 0.1f;
		graphics::playSound(std::string(ASSET_PATH) + "passed.mp3", 0.8f, false);
	}
	if (gameLife < 0) {
		Status = 2;
		graphics::playSound(std::string(ASSET_PATH) + "lost.mp3", 0.2f, false);
	}
}
void Game::init() {
	levels = 10;
	gameLife = 1.0f;
	enemyTime = 0.f;
	enemyPosition = 0;
	maxEnemies = 3;
	boostTimer = 0.0f;
	score = 0;
	Status = 0;
	graphics::setFont(std::string(ASSET_PATH) + "spacetext.ttf");
	graphics::playMusic(std::string(ASSET_PATH) + "background.mp3", 0.3f, true, 3000);
}
Game::Game() {

}
Game::~Game(){
		if (player){
			delete player;
		}
	}