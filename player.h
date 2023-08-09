#pragma once
#include "gameobj.h"
#include "bullet.h"

class Player: public GameObj{
protected:
	float bulletTime=0.0f;
	int life = 3;
	bool shielded = false;
	bool noReload = false;
	float shieldTimer;
	float noReloadTimer;
	float glow;
	float hit_timer;
public:
	Bullet* bullet = nullptr;
	std::vector< Bullet*> bullets;
	void draw() override;
	void update() override;
	void init() override;
	Disk getCollisionHull() const override;
	void setLife(bool);
	int getLife();
	void setShield();
	bool isShielded();
	void setnoReload();
	float getShieldTimer();
	Player();
	~Player();
};