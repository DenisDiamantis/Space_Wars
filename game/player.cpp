#include "player.h"
#include "graphics.h"
#include <iostream>

void Player::draw()
{	//DRAW SPACESHIP

	if ((hit_timer) < graphics::getGlobalTime())
		glow = 1.0f;
	else
		glow = 0.5f + 0.5f * sin(graphics::getGlobalTime() / 50.0f);
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.fill_opacity = 1.0f;
	br.fill_color[1] = 1.0f * glow;
	br.fill_color[2] = 1.0f * glow;
	br.texture=std::string(ASSET_PATH)+"spaceship.png";
	graphics::drawRect(pos_x,pos_y,size,size,br);
	//DRAW SHIELD 
	if (shielded) {
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		if ((shieldTimer - 1500.0f) < graphics::getGlobalTime())
			glow = 0.5f + 0.5f * sin(graphics::getGlobalTime() / 50.0f);
		else
			glow = 1.0f;
		br.outline_opacity = 0.0f;
		br.texture = std::string(ASSET_PATH) + "shield.png";
		br.fill_opacity = 0.3f + 0.7f * glow;
		graphics::drawRect(pos_x, pos_y, size+size/2, size+size/2, br);
	}
	//DRAW BULLETS
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isAlive())
			bullets[i]->draw();	
	}
	//DRAW COLLISION IF DEBUG MODE TRUE
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
	br.fill_secondary_color[0] = 0.0f;
	br.fill_secondary_color[1] = 0.0f;
	br.fill_secondary_color[2] = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	graphics::drawDisk(pos_x - size / 2.0f,pos_y-size/2.7f,20,br);
	graphics::drawDisk(pos_x - size / 2.0f, pos_y + size / 2.7f, 20, br);
}

void Player::update()
{	//PLAYER MOVEMENT
	if(graphics::getKeyState(graphics::SCANCODE_A)){
		pos_x -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if(graphics::getKeyState(graphics::SCANCODE_D)){
		pos_x += speed * graphics::getDeltaTime() / 10.0f;
	}
	if(graphics::getKeyState(graphics::SCANCODE_W)){
		pos_y -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if(graphics::getKeyState(graphics::SCANCODE_S)){
		pos_y += speed * graphics::getDeltaTime() / 10.0f;
	} 
	if ((pos_x - size/2.0f) < 0) pos_x = size / 2.0f;
	if ((pos_x + size/2.0f) > CANVAS_WIDTH) pos_x = CANVAS_WIDTH - size / 2.0f;
	if ((pos_y - size/2.0f) < 0) pos_y = size / 2.0f;
	if ((pos_y + size/2.0f) > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT - size / 2.0f;
	//PLAYER SHOOTING BULLETS WITH TIME LIMIT
	if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
		if (graphics::getGlobalTime() > bulletTime) {
			bullet = new Bullet(pos_x + size / 2.0f, pos_y,true);
			bullets.push_back(bullet);
			graphics::playSound(std::string(ASSET_PATH) +"bullet.mp3", 0.3f, false);
			if(noReload) bulletTime = graphics::getGlobalTime() + 100.0f;
			else bulletTime = graphics::getGlobalTime() + 500.0f;
		}
	}	
	//UPDATE BULLETS
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isAlive())
			bullets[i]->update();
	}
	
	if (shielded && shieldTimer < graphics::getGlobalTime()) {
		shielded = false;
	}
	if (noReload && noReloadTimer < graphics::getGlobalTime()) {
		noReload = false;
	}
}

void Player::init() {
	hit_timer=0.0f;
	shieldTimer=0.0f;
	noReloadTimer=0.0f;
	speed = 1.5f;
	size = 100.0f;
	pos_x = size / 2.0f;
	pos_y = CANVAS_HEIGHT / 2.0f;
}

void Player::setLife(bool hit)
{	//IF PLAYER IS HIT TRUE
	if (!shielded && hit) {
		life -= 1;
		hit_timer=graphics::getGlobalTime() + 1000.0f;
	}
	if (!hit&&!(life==3)) life += 1;
	
}

int Player::getLife()
{
	return life;
}

void Player::setShield()
{	//SET SHIELD FROM BOOST OR HIT
	shielded = true;
	shieldTimer = graphics::getGlobalTime() + 3000.0f;
}

bool Player::isShielded()
{
	return shielded;
}

void Player::setnoReload()
{//BOOST
	noReload = true;
	noReloadTimer = graphics::getGlobalTime() + 3000.0f;
}

float Player::getShieldTimer()
{
	return shieldTimer;
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size/2.0f+size/20.0f;
	return disk;
}

Player::Player()
{
	init();
}

Player::~Player()
{
}
