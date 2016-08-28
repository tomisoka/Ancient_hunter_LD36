#pragma once
#include "enemy.h"

class Bear: public Enemy{
public:
	Bear(QVector2D pos);

	virtual void reaction(QVector2D pos, int type = 0);
	virtual void tick();
	virtual void render(QPainter *painter);
protected:
	virtual void crashed();

	void attack(QVector2D pos);
};
