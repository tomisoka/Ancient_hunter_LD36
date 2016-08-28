#pragma once
#include "common.h"

class Trader: public QObject{
	Q_OBJECT
public:
	Trader(QVector2D pos, int ID);
	virtual ~Trader();

	virtual void tick();
	virtual void render(QPainter *painter);
	virtual void renderShop(QPainter *painter);

	void setActive(bool active){this->active = active;}


	int getH(){return h;}
	int getW(){return w;}

	QVector2D pos;

	void setShopping(bool shopping);

public slots:
	void keyPress(QKeyEvent *ev);
	void keyRelease(QKeyEvent *ev);

protected:
	int chosen;

	bool toAudio;

	bool shopping;

	int ID;

	bool active;

	int h,w;
};


void setup_trader();
