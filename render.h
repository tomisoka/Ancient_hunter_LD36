#pragma once
#include "window.h"

class renderWindowSizeUpdater: public QObject{
	Q_OBJECT
public:
	renderWindowSizeUpdater():toResetSize(0){}
	void resetSize();
public slots:
	void resize(int SZ);
private:
	bool toResetSize;
};

extern QBackingStore *backingStore;

void resetRenderWindowSizeUpdater();

void setup_render();
void cleanup_render();
