#include "render.h"

QBackingStore *backingStore;

void renderWindowSizeUpdater::resize(int SZ){
	Q_UNUSED(SZ);
	toResetSize = 1;
}

void renderWindowSizeUpdater::resetSize(){
	if(toResetSize){
		backingStore->resize(QSize(window->width(), window->height()));
		toResetSize = 0;
	}
}

renderWindowSizeUpdater *RenderWindowSizeUpdater;

void resetRenderWindowSizeUpdater(){
	RenderWindowSizeUpdater->resetSize();
}

void setup_render(){
	RenderWindowSizeUpdater = new renderWindowSizeUpdater();
	backingStore = new QBackingStore(window);
	backingStore->resize(QSize(window->width(), window->height()));

	QObject::connect(window, &QWindow::widthChanged, RenderWindowSizeUpdater, &renderWindowSizeUpdater::resize);
	QObject::connect(window, &QWindow::heightChanged, RenderWindowSizeUpdater, &renderWindowSizeUpdater::resize);
}

void cleanup_render(){
	delete RenderWindowSizeUpdater;
	delete backingStore;
}
