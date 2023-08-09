#include "boostUps.h"

int BoostUps::getType()
{
	return type;
}

void BoostUps::draw()
{
	//DRAW HEART BOOST
	if (type == 0) {
		br.texture = std::string(ASSET_PATH) + "heartshield.png";
		br.fill_opacity = 1.0f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, br);
	}
	//DRAW BULLETS BOOST
	else if (type == 1) {
		br.texture = std::string(ASSET_PATH) + "reload.png";
		br.fill_opacity = 1.0f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, br);
	}
	//DRAW TIME SLOW BOOST
	else {
		br.texture = std::string(ASSET_PATH) + "clock.png";
		br.fill_opacity = 1.0f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, br);
	}	//DRAW COLLISION
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

void BoostUps::update()
{	//UPDATE BOOST 
	pos_x -= speed * graphics::getDeltaTime();
	if (pos_x < 0) alive = false;
}

void BoostUps::init()
{
	speed = 0.2f;
	pos_x = CANVAS_WIDTH + 100;
	pos_y = CANVAS_HEIGHT * rand0to1();
	size = 50;
	//HEIGHT BOUNDS CHECK
	if ((pos_y - size / 2.0f) < 0) pos_y = size / 2.0f;
	if ((pos_y + size / 2.0f) > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT - size / 2.0f;
}

Disk BoostUps::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 2.0f + size / 20.0f;
	return disk;
}

bool BoostUps::isAlive()
{
	return alive;
}

BoostUps::BoostUps(int boostType)
{
	type = boostType;
	init();
}

BoostUps::~BoostUps()
{
}
