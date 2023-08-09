#pragma once
#include "graphics.h"
#include "player.h"
class Enemy: public Player
{
	bool timeSlow;
	float slowTimer;
	float explosionTimer;
	float active;
public:
	
	float getExplosionTimer();;
	float getY();
	float getX();
	void draw();
	void update();
	void init(float);
	bool isAlive();
	bool isActive();
	void setnotAlive();
	void setTimeSlow();
	float getSlowTimer();
	Disk getCollisionHull() const;
	Enemy(float);
	~Enemy();
};