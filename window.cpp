#include "window.h"

Window *window;
CloseEventFilter *closeFilter;


void setup_window(){
	window = new Window();
	window->setTitle(APPNAME);


	window->setMinimumHeight(wMinHeight);
	window->setMinimumWidth(wMinWidth);
	window->resize(wWidth, wHeight);


	window->setSurfaceType(QWindow::SurfaceType::RasterSurface);


	closeFilter = new CloseEventFilter(window);
	window->installEventFilter(closeFilter);

	window->show();
}

void cleanup_window(){
	delete window;
}
