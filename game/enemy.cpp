#include "enemy.h"
#include "config.h"
#include "util.h"



float Enemy::getY()
{
	return pos_y;
}

float Enemy::getExplosionTimer()
{
	return explosionTimer;
}

void Enemy::draw()
{	//DRAW SPACESHIP
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "enemy.png";
	br.fill_opacity = 1.0f;
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
	//DRAW BULLETS
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isAlive())
			bullets[i]->draw();

	}
	//DRAW COLLISION
	if (DEBUG_MODE) {

		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[0] = 0.3f;
		br.fill_opacity = 0.3f;
		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	}
	//DRAW JET ENGINE
	glow = 0.5f + 0.5f * sin(graphics::getGlobalTime());
	br.fill_opacity = 1.0f;
	br.texture = "";
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 0.5f + glow * 0.5f;
	br.fill_color[1] = 0.5f + glow * 0.5f;
	br.fill_color[2] = 0.5f + glow * 0.5f;
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.0f;
	br.fill_secondary_color[2] = 0.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	graphics::drawDisk(pos_x + size / 2.0f, pos_y, 20, br);
	//DRAW FIRE WHEN HIT
	if (!alive) {
		glow = 0.5f + 0.5f * sin(graphics::getGlobalTime() / 50.0f);
		graphics::Brush ebr;
		ebr.texture = std::string(ASSET_PATH) + "explosion.png";
		ebr.fill_opacity = 0.5f + 0.5f * glow;
		ebr.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, 50, 50, ebr);
	}
	
}

void Enemy::update()
{	
	if (alive) {
		//SPAWN BULLETS
		if (graphics::getGlobalTime() > bulletTime) {
			bullet = new Bullet(pos_x - size / 2.0f, pos_y, false);
			bullets.push_back(bullet);
			bulletTime = graphics::getGlobalTime() + 4000.0f;
		}
		//UPDATE BULLETS
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->isAlive())
				bullets[i]->update();
		}
		//TIME SLOW BULLETS
		if (timeSlow) {
			for (int i = 0; i < bullets.size(); i++) {
				bullets[i]->setTimeSlow(slowTimer);
			}
		}
		//TIME SLOW MOVEMENT
		if (!timeSlow) pos_x -= speed * graphics::getDeltaTime();
		else pos_x -= speed / 2 * graphics::getDeltaTime();
		//IF OUT OF BOUNDS
		if (pos_x < 0) active = false;
		//RESET TIME SLOW
		if (timeSlow && graphics::getGlobalTime() > slowTimer) {
			timeSlow = false;
		}
	}
	else {
		//IF HIT STOP MOVEMENT 
		pos_x = pos_x;
		if (explosionTimer < graphics::getGlobalTime()) active = false;
	}
}
bool Enemy::isActive()
{
	return active;
}

void Enemy::init(float pos)
{
	bulletTime = graphics::getGlobalTime() + 2000.0f;
	explosionTimer = 0.0f;
	slowTimer = 0.0f;
	timeSlow = false;
	speed = 0.1f;
	pos_x = CANVAS_WIDTH+100;
	pos_y = CANVAS_HEIGHT * rand0to1();
	active = true;
	size = 75;
	//CHECK ENEMY SPAWN POSITION HEIGHT SO THEY DON'T OVERLAP AND HEIGHT BOUNDS CHECK
	do {
		if (abs(pos_y - pos) < 100) pos_y= CANVAS_HEIGHT * rand0to1();
		if ((pos_y - size / 2.0f) < 0) pos_y = size / 2.0f;
		//TO POSITION ABOVE THE GAME UI
		if ((pos_y + size / 2.0f) > CANVAS_HEIGHT-size) pos_y = CANVAS_HEIGHT - size;
	} while (abs(pos_y - pos) < 100);
}

bool Enemy::isAlive()
{
	return alive;
}

void Enemy::setnotAlive()
{	//IF HIT SET TIME TO SHOW DEATH EFFECT
	alive = false;
	explosionTimer = graphics::getGlobalTime() + 500;
}

float Enemy::getX()
{
	return pos_x;
}

Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 2.0f + size / 20.0f;;
	return disk;
}

void Enemy::setTimeSlow()
{	//SET TIME SLOW BOOST
	timeSlow = true;
	slowTimer= graphics::getGlobalTime() + 3000.0f;
}

Enemy::Enemy(float pos)
{
	init(pos);
}

float Enemy::getSlowTimer()
{
	return slowTimer;
}

Enemy::~Enemy()
{
}
