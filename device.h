#pragma once
#include "common.h"

class Device: public QObject{
	Q_OBJECT
public:
	Device(QVector2D pos);
	virtual ~Device();

	virtual void tick(){}
	virtual void render(QPainter *painter){}

	void setActive(bool active){this->active = active;}


	int getH(){return h;}
	int getW(){return w;}

	QVector2D pos;

protected:
	bool active;

	int h,w;
};
