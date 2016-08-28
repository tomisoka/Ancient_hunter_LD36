#pragma once
#include "common.h"

class Enemy: public QObject{
	Q_OBJECT
public:
	Enemy(QVector2D pos);
	virtual ~Enemy();

	virtual void attacked(int force){HP-=force; if(HP <= 0)killed();}
	virtual void killed();


	virtual void reaction(QVector2D pos, int type = 0){}

	virtual void tick();
	virtual void render(QPainter *painter);

	void setActive(bool active){this->active = active;}

	int getFood(){return foodValue;}

	int getH(){return h;}
	int getW(){return w;}

	QVector2D pos;

	bool myReaction;
public slots:

protected:
	int looped;

	virtual void tickMove();
	virtual void crashed(){}

	int halfSZ;

	bool active;

	int h,w;

	int foodValue;
	int HP;
	int attackDelay;

	QVector2D move;
	QVector2D lastMove;
};
