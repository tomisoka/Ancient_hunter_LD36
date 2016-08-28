#pragma once

#include "common.h"
#include "device.h"

class CampFire: public Device{
public:
	CampFire(QVector2D pos);

	void tick();
	void render(QPainter *painter);
private:
	int tickedN;
};
