#pragma once

#include "common.h"
#include "trader.h"

class Player: public QObject{
	Q_OBJECT
public:
	Player();
	~Player();

	void reset();

	void attacked(int force);

	int getW(){return w;}
	int getH(){return h;}

	void tick();
	void render(QPainter *painter);

	bool isMoving(){return move != QVector2D(0,0); }

	void setActive(bool active){this->active = active; moveLeft = 0; moveUp = 0; moveRight = 0; moveDown = 0; attack = 0;}
	Trader *getTrader(){return trader;}

	QVector2D pos;
public slots:
	void keyPress(QKeyEvent *ev);
	void keyRelease(QKeyEvent *ev);

protected:
	int bowDist;

	int attackedDelay;

	int weapon;



	int halfSZ;

	int HP;
	int maxHP;
	int stamina;
	int maxStamina;

	int loadStamina;


	void tickAttack();
	void tickMove();
	void tickCommunication();

	int h,w;

	QVector2D move;
	QVector2D lastMove;


	int renderAttack;


	int bowDistChange;
	bool sprint;
	bool active;
	bool attack;
	bool moveLeft;
	bool moveUp;
	bool moveRight;
	bool moveDown;

	bool communicate;

	Trader *trader;
};
