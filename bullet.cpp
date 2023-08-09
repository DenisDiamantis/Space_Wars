#include "bullet.h"
#include "config.h"
#include "graphics.h""

void Bullet::draw()
{
	br.outline_opacity = 0.0f;
	//DRAW PLAYER BULLETS
	if (playerBullet) {
		br.texture = std::string(ASSET_PATH) + "bullet.png";
		graphics::drawRect(pos_x, pos_y, size, size / 2.0f, br);
	}
	//DRAW ENEMY BULLETS
	else {
		br.texture = std::string(ASSET_PATH) + "bullet2.png";
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size / 2.0f, br);
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
}

void Bullet::update()
{	//UPDATE PLAYER BULLET
	if (playerBullet)
	{
		pos_x += speed * graphics::getDeltaTime();
		if (pos_x > CANVAS_WIDTH) alive = false;
	}
	//UPDATE ENEMY BULLET OR TIME SLOW BULLET
	else {
		if(!timeSlow) pos_x -= speed * graphics::getDeltaTime();
		else pos_x -= speed/2 * graphics::getDeltaTime();
		if (pos_x < 0) alive = false;
	}
	if (timeSlow && graphics::getGlobalTime() > slowTimer) {
		timeSlow=false;
	}
}

void Bullet::init()
{	
	speed = 0.2f;
	size = 50;
	timeSlow = false;
	slowTimer = 0.0f;
	
}

Disk Bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 4.0f;
	return disk;
}

void Bullet::setTimeSlow(float restTime)
{
	timeSlow = true;
	slowTimer = restTime;
}

bool Bullet::isAlive()
{
	return alive;
}
void Bullet::setnotAlive()
{
	alive = false;
}
Bullet::Bullet(float x, float y,bool identity)
{
	pos_x = x;
	pos_y = y;
	playerBullet = identity;
	init();

}

float Bullet::getSlowTimer()
{
	return slowTimer;
}

Bullet::~Bullet()
{
}
