#pragma once
#include "gameobj.h"

class BoostUps :public GameObj {
	int type;
public:
	int getType();
	void draw() override;
	void update() override;
	void init() override;
	Disk getCollisionHull() const override;
	bool isAlive();
	BoostUps(int);
	~BoostUps();
};