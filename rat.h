#pragma once
#include "enemy.h"

class Rat: public Enemy{
public:
	Rat(QVector2D pos);

	virtual void reaction(QVector2D pos, int type = 0);
	virtual void tick();
	virtual void render(QPainter *painter);
protected:
	virtual void crashed();
};
