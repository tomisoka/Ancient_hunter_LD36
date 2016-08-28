#pragma once
#include "device.h"


class Trap: public Device{
public:
	Trap(QVector2D pos);

	void tick();
	void render(QPainter *painter);
private:
};
