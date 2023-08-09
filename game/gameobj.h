#pragma once
#include "util.h"
#include "graphics.h"
#include "config.h"

class GameObj 
{	protected:
	float speed;
	float size;
	float pos_x, pos_y;
	graphics::Brush br;
	bool alive = true;
public:
	virtual void draw()=0;
	virtual void update()=0;
	virtual void init()=0;
	virtual Disk getCollisionHull() const = 0;
};
