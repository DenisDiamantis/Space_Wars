#pragma once
#include "gameobj.h"

class Bullet: public GameObj
{
	bool timeSlow;
	bool playerBullet;
	float slowTimer;
public:
	void draw() override;
	void update() override;
	void init() override;
	bool isAlive();
	void setnotAlive();
	void setTimeSlow(float);
	float getSlowTimer();
	Disk getCollisionHull() const;
	Bullet(float,float,bool);
	~Bullet();
};